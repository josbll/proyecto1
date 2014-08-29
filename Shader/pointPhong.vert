#version 110



uniform vec3 Color;

layout ( location = 0 ) out vec4 FragColor;

uniform vec3 code;
uniform vec3 lightposition;
flat in vec3 lightintensity;
flat in vec3 normal;
flat in vec3 pos;
flat in vec3 lightPos;

vec3 Phong(vec3 Kd,vec3 Ld,vec3 Ka, vec3 La,vec3 Ks,vec3 Ls,vec3 LightDir,vec3 V,float shinness,float att)
    {
       vec3 N=normalize(normal);
       vec3 R=normalize(reflect(LightDir,N));     
      vec3 diffuse=Ld*Kd*max(dot(LightDir,N),0.0);
      vec3 ambient=Ka*La;
      vec3 specular=Ls*Ks*pow(max(dot(V,R),0.0),shinness);
         vec3 color=att*diffuse+att*specular+att*ambient;
         
         return color; 
    }
	
void main()
{   
   vec3 Kd=vec3(1.0,1.0,1.0);
   vec3 Ld=vec3(0.5,0.2,0.1);
   vec3 La=vec3(0.4,0.2,0.1);
   vec3 Ka=vec3(1.0,0.0,0.0);
   vec3 Ks=vec3(0.6,0.2,0.1);
   vec3 Ls=vec3(0.6,0.2,0.0);
   vec3 lightDir=vec3(2.0,0.0,0.0);
   vec3 V=vec3(3.4,1.4,0.0);
  
     V=normalize(lightposition-pos);
	  vec3 h=normalize(V+lightposition);
	 float dist=length(V);
	 float NdotL=max(dot(normal,V),0.0);
	 vec3 color=vec3(0.4,0.4,0.0);
	 float att;
	 vec3 light;
	 /* if(NdotL>0.0)
	    {*/
		  att=1.0/1.0;
		
		//}
     light=Phong(Kd,Ld,Ka,La,Ks,Ls,lightposition,V,4.0,att);
	gl_FragColor = vec4(light,1.0);
} 
