#version 330														
					
// In Variables
in vec4 vColor;														
in vec2 TexCoord;
in vec3 Normal;
in vec3 FragPos;


// Out Variables
out vec4 color;														


// Constants
const int MAX_POINT_LIGHTS = 3;
const int MAX_SPOT_LIGHTS = 3;


// Data Structs 
struct Light
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
uniform Material material;

uniform vec3 eyePosition;


// Methods
// Base light calculation (ambient, diffuse, specular)
vec4 CalcLightByDirection(Light light, vec3 direction)
{
	// Ambient Calculation
	vec4 ambientColor = vec4(light.color, 1.0f) * light.ambientIntensity;

	// Diffuse Calculation
	float diffuseFactor = max(dot(normalize(Normal), normalize(direction)), 0.0f);
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
	return (ambientColor + diffuseColor + specularColor);		
}


// Simply the base light calculation
vec4 CalcDirectionalLight()
{
	return CalcLightByDirection(directionalLight.base, directionalLight.direction);
}


// Calculate single point light ((ambient + diffuse + specular) / attenuation)
vec4 CalcPointLight(PointLight pLight)
{
	// Calculate direction and distance of frag to point light
	vec3 direction = FragPos - pLight.position;
	float distance = length(direction);
	direction = normalize(direction);

	// Base point light value is simply the directional light calculation
	vec4 color = CalcLightByDirection(pLight.base, direction);

	// Compute attenuation for "drop-off" factor
	float attenuation = pLight.exponent * distance * distance +
						pLight.linear * distance + 
						pLight.constant;

	// Final point light value is directional light divided by attenuation
	return color / attenuation;
}


// Calculate single spot light ((ambient + diffuse + specular) / attenuation) within light cone
vec4 CalcSpotLight(SpotLight sLight)
{
	// Calculate direction from spotlight to fragment
	vec3 rayDirection = normalize(FragPos - sLight.base.position);
	// Calculate angle between light ray direction and fragment to light
	float slFactor = dot(rayDirection, sLight.direction);

	// if fragment is within light cone of spot light, calculate as point light
	if (slFactor > sLight.edge)
	{
		vec4 color = CalcPointLight(sLight.base);

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
		totalColor += CalcPointLight(pointLights[i]);
	}

	return totalColor;
}

// Iterate through each spot light, calling calc method
vec4 CalcSpotLights()	
{
	vec4 totalColor = vec4(0, 0, 0, 0);

	for (int i = 0; i < spotLightCount; i++)
	{
		totalColor += CalcSpotLight(spotLights[i]);
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