#version 330														
					
// Data in from vertex shader 
in vec4 vColor;														
in vec2 TexCoord;
in vec3 Normal;
in vec3 FragPos;
in vec4 DirectionalLightSpacePos;

// Out Variables
out vec4 color;														

// Constants
const int MAX_POINT_LIGHTS = 3;
const int MAX_SPOT_LIGHTS = 3;

// Data Structs 

struct Light // base class
{
	vec3 color;
	float ambientIntensity;
	float diffuseIntensity;
};

struct DirectionalLight 
{
	Light base;
	vec3 direction;
};

struct PointLight
{
	Light base;
	vec3 position;
	float constant;
	float linear;
	float exponent;
};

struct SpotLight
{
	PointLight base;
	vec3 direction;
	float edge;
};

struct OmniShadowMap
{
	samplerCube shadowMap;  // texture, vector pointing to point on shadow map layer
	float farPlane;			// distance shadow map reaches
};

struct Material
{
	float specularIntensity;
	float shininess;
};


// Uniforms
uniform int pointLightCount;
uniform int spotLightCount;

uniform DirectionalLight directionalLight;
uniform PointLight pointLights[MAX_POINT_LIGHTS];
uniform SpotLight spotLights[MAX_SPOT_LIGHTS];

uniform sampler2D theTexture;
uniform sampler2D directionalShadowMap;
uniform OmniShadowMap omniShadowMaps[MAX_POINT_LIGHTS + MAX_SPOT_LIGHTS]; // First half of array will match indices with pointLights array, second with spotLights array

uniform Material material;

uniform vec3 eyePosition;

// vectors out from center pixel to determine sampling for shadow cube PCF
vec3 sampleOffsetDirections[20] = vec3[]
(
	vec3(1, 1, 1), vec3(1, -1, 1), vec3(-1, -1, 1), vec3(-1, 1, 1), 
	vec3(1, 1, -1), vec3(1, -1, -1), vec3(-1, -1, -1), vec3(-1, 1, -1), 
	vec3(1, 1, 0), vec3(1, -1, 0), vec3(-1, -1, 0), vec3(-1, 1, 0), 
	vec3(1, 0, 1), vec3(-1, 0, 1), vec3(1, 0, -1), vec3(-1, 0, -1), 
	vec3(0, 1, 1), vec3(0, -1, 1), vec3(0, -1, -1), vec3(0, 1, -1) 
);

// Methods:

// Check if current fragment is the closest fragment to the light in the light's line of sight. Set shadow accordingly
float CalcDirectionalShadowFactor(DirectionalLight light)
{
	vec3 projCoords = DirectionalLightSpacePos.xyz / DirectionalLightSpacePos.w; // transform light space position into clip space [-1.0, 1.0]
	projCoords = (projCoords * 0.5) + 0.5; // tranform to [0.0, 1.0] depth range

	// Get the shadow map depth at the current position (shadow map is composed of closest depths)
	// shadowMap texture only has one value: the depth (r). To get that value at the corresponding directional light position reference projCoords.xy
	float currentDepth = projCoords.z;									 // depth of current fragment

	// remove shadow acne
	vec3 normal = normalize(Normal);
	vec3 lightDir = normalize(light.direction);
	
	// set bias. May need to be adjusted per light
	float bias = max(0.05 * (1 - dot(normal, lightDir)), 0.005);  // Range [0.5, 0.005], function of angle between normal and light direction

	// PCF: Percentage closest filtering to smooth shadow edges
	float shadow = 0.0;

	// get single texel size in relation to overall texture
	vec2 texelSize = 1.0 / textureSize(directionalShadowMap, 0); // textureSize args: texture, mipmap level
	
	// iterate through 9 texels centered on current texel
	for (int x = -2; x <= 2; ++x)
	{
		for (int y = -2; y <= 2; ++y)
		{
			// depth of current and surrounding texels
			float pcfDepth = texture(directionalShadowMap, projCoords.xy + vec2(x, y) * texelSize).r;

			// if current depth is greater than the shadow map depth, set full shadow, else no shadow
			shadow += currentDepth - bias > pcfDepth ? 1.0 : 0.0;
		}
	}

	shadow /= 25.0f; // average of current and surrounding texels
	// end PCF and bias solutions

	// if fragment is beyond far plane, set no shadow
	if (projCoords.z > 1.0)
	{
		shadow = 0.0; 
	}

	return shadow;
}

float CalcOmniShadowFactor(PointLight light, int shadowIndex)
{
	vec3 fragToLight = FragPos - light.position; // vector from fragment to light (depth of fragment from light)
	float currentDepth = length(fragToLight);	 // get length of current fragment

	float shadow = 0.0;
	float bias = 0.05f;							 // biase to negate shadow acne
	int samples = 20;
	
	float viewDistance = length(eyePosition - FragPos);                                      // distance between eye and point being rendered
	float diskRadius = (1.0 + (viewDistance / omniShadowMaps[shadowIndex].farPlane)) / 25.0; // vary amount of pixels sampled according to view distance

	// PCF to smooth shadow edges
	for (int i = 0; i < samples; i++)
	{
		// get fragment with smallest depth (closest to light)
		float closestDepth = texture(omniShadowMaps[shadowIndex].shadowMap, fragToLight + sampleOffsetDirections[i] * diskRadius).r;
		closestDepth *= omniShadowMaps[shadowIndex].farPlane; // undo normalization of depth for vector length (occured in omni_shadow_map.frag)
		if (currentDepth - bias > closestDepth)
		{
			shadow += 1.0;
		}

	}

	shadow /= float(samples); // get average of surrounding shadows
	return shadow;
}

// Base light calculation (ambient, diffuse, specular) adjusted for shadow
vec4 CalcLightByDirection(Light light, vec3 direction, float shadowFactor)
{
	// Ambient Calculation
	vec4 ambientColor = vec4(light.color, 1.0f) * light.ambientIntensity;

	// Diffuse Calculation
	float diffuseFactor = max(dot(normalize(Normal), normalize(direction)), 0.0f); // usually, direction is negated as well as normalized
	vec4 diffuseColor = vec4(light.color * light.diffuseIntensity * diffuseFactor, 1.0f);

	// Specular Calculation
	vec4 specularColor = vec4(0, 0, 0, 0);

	if(diffuseFactor > 0.0f)
	{
		// Calculate direction from fragment to "eye"
		vec3 fragToEye = normalize(eyePosition - FragPos);
		// Calculate light direction after bouncing off fragment (reflected around fragment normal)
		vec3 reflectedVertex = normalize(reflect(direction, normalize(Normal)));
		// Calculate angle difference from fragment to eye and light direction after bounce
		float specularFactor = dot(fragToEye, reflectedVertex);

		// If light is reflected close enough towards eye, calculate specular factor
		if (specularFactor > 0.0f)
		{
			// intensity of specular light is a power of material "shininess" (usually the power is a multiple of 2)
			specularFactor = pow(specularFactor, material.shininess);
			// specularIntensity is how much light to apply
			specularColor = vec4(light.color * material.specularIntensity * light.diffuseIntensity * specularFactor, 1.0f);
		}
	}

	// Total light is the sume of the components
	return (ambientColor + (1.0 - shadowFactor) * (diffuseColor + specularColor));		
}


// Simply the base light calculation
vec4 CalcDirectionalLight()
{
	float shadowFactor = CalcDirectionalShadowFactor(directionalLight); // check if fragment is in shadow
	return CalcLightByDirection(directionalLight.base, directionalLight.direction, shadowFactor);
}


// Calculate single point light ((ambient + diffuse + specular) / attenuation)
vec4 CalcPointLight(PointLight pLight, int shadowIndex)
{
	// Calculate direction and distance of frag to point light
	vec3 direction = FragPos - pLight.position;
	float distance = length(direction);
	direction = normalize(direction);

	float shadowFactor = CalcOmniShadowFactor(pLight, shadowIndex); // Set shadow factor to 1.0 or 0 after calculating if fragment is in shadow

	// Base point light value is simply the directional light calculation
	vec4 color = CalcLightByDirection(pLight.base, direction, shadowFactor); 

	// Compute attenuation for "drop-off" factor
	float attenuation = pLight.exponent * distance * distance +
						pLight.linear * distance + 
						pLight.constant;

	// Final point light value is directional light divided by attenuation
	return color / attenuation;
}


// Calculate single spot light ((ambient + diffuse + specular) / attenuation) within light cone
vec4 CalcSpotLight(SpotLight sLight, int shadowIndex)
{
	// Calculate direction from spotlight to fragment
	vec3 rayDirection = normalize(FragPos - sLight.base.position);
	// Calculate angle between light ray direction and fragment to light
	float slFactor = dot(rayDirection, sLight.direction);

	// if fragment is within light cone of spot light, calculate as point light
	if (slFactor > sLight.edge)
	{
		vec4 color = CalcPointLight(sLight.base, shadowIndex);

		// color * spot light blending factor 
		// SLBF = greatestVal - (greatestVal - curVal) * (ratioConversion: (1.0 to 0.0) / (1.0 to about 0.98))
		return color * (1.0f - (1.0f - slFactor) * (1.0f / (1.0f - sLight.edge)));
	}
	else
	{
		return vec4(0, 0, 0, 0);
	}
}


// Iterate through each point light, calling calculation method
vec4 CalcPointLights()
{
	vec4 totalColor = vec4(0, 0, 0, 0);
	for (int i = 0; i < pointLightCount; i++)
	{
		totalColor += CalcPointLight(pointLights[i], i); // omniShadowMaps and pointLights arrays have matching indices
	}

	return totalColor;
}

// Iterate through each spot light, calling calc method
vec4 CalcSpotLights()	
{
	vec4 totalColor = vec4(0, 0, 0, 0);

	for (int i = 0; i < spotLightCount; i++)
	{
		totalColor += CalcSpotLight(spotLights[i], i + pointLightCount); // omniShadowMaps and SpotLights arrays have matching indices + offset of size of pointLights array
	}

	return totalColor;
}

void main()															
{	
	vec4 finalColor = CalcDirectionalLight();
	finalColor += CalcPointLights();
	finalColor += CalcSpotLights();

	// Apply light to texture as a factor
	color = texture(theTexture, TexCoord) * finalColor;													
}