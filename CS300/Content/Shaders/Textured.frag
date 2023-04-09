#version 460 core

struct MaterialParameters {
	vec4 emission;
	vec4 ambient;
	vec4 diffuse;
	vec4 specular;
	float shininess;
};

struct Light {
    vec3 pos;
    vec3 dir;
    vec3 amb;
    vec3 dif;
    vec3 spe;
    vec3 att;
    float cosIn;
    float cosOut;
    float fallOff;
};

uniform MaterialParameters uMaterial;
uniform Light uLight[8];
uniform vec3 uCameraPos;
uniform mat4 uTransform;
uniform mat4 uView;
uniform mat4 uModel;
uniform int uLightCount;

out vec4 FragColor;
in vec2 oUVs;
in vec3 oNormal;
in vec3 oPosition;

uniform sampler2D colorsTex;

void main() {
    vec3 totalLightShine = vec3(0, 0, 0);
    
    for(int i = 0; i < 1; i++) {
        //ambient
        float ambientStrength = 0.1;
        vec3 ambient = uLight[i].amb;
   
        //diffuse
        vec3 norm = normalize(mat3(transpose(inverse(uModel))) * oNormal);
        vec3 lightDir = normalize(uLight[i].pos - oPosition); 
        float diff = max(dot(norm, lightDir), 0.0);
        vec3 diffuse = diff * uLight[i].dif;
    
         //specular
        vec3 viewDir = normalize(uCameraPos - oPosition);
        vec3 reflectDir = reflect(-lightDir, norm);
        float spec = pow(max(dot(viewDir, reflectDir), 0.0), 32);
        vec3 specular = uLight[i].spe * spec;

        float dist = length(lightDir);
        float att = min(1.f / (uLight[i].att.x + uLight[i].att.y * dist + uLight[i].att.z * dist * dist), 1.0f);
    
       float Spotlight =1;
		vec3 d = normalize(uLight[i].dir);
		//if(type[i]==1)
		//{
			float aplha = dot(-lightDir, d);

			if(aplha < cos(uLight[i].cosOut))
				Spotlight = 0;
			else if(aplha > cos(uLight[i].cosIn))
				Spotlight = 1;
			else
				Spotlight= pow((aplha-cos(uLight[i].cosOut))/(cos(uLight[i].cosIn)-cos(uLight[i].cosOut)), uLight[i].fallOff);

			Spotlight= clamp(Spotlight,0,1);
		//}


        totalLightShine += att * (ambient + Spotlight * (diffuse + specular));
   }
  
    FragColor =  vec4(totalLightShine, 1.0);
}