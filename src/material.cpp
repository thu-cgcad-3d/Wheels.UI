#include "material.hpp"

#include "wheels/tensor.hpp"

namespace wheels {
material make_simple_material(const vec3f &color) {
  return make_simple_material(image3f32(make_shape(1, 1), color));
}
material make_simple_material(image3f32 &&diffuse_map) {
  material mat;
  mat.compute_color_function = R"SHADER(
     vec4 ComputeColor(int valid_lights_num, 
       vec3 frag_pos, vec3 normal, vec2 tex_coord){
       
       vec3 color = vec3(0.9);
       if (tex_diffuse.exists) {
         color = texture(tex_diffuse.tex, tex_coord).rgb;
       }    
       vec3 lighting_sum = vec3(0.0);       
       
       for(int light_id = 0; light_id < valid_lights_num; ++light_id){
         vec3 light_color = light_colors[light_id];
         vec3 light_dir = normalize(light_positions[light_id] - frag_pos);      
         vec3 view_dir = normalize(eye - frag_pos);
         vec3 reflect_dir = reflect(-light_dir, normal);
         vec3 halfway_dir = normalize(light_dir + view_dir);  
         float shadow = ComputeShadow(light_id); 
       
         // Ambient
         vec3 ambient = 0.3 * color;          
       
         // Diffuse
         float diff = max(dot(light_dir, normal), 0.0);
         vec3 diffuse = diff * light_color;
       
         // Specular         
         float spec = 0.0;          
         spec = pow(max(dot(normal, halfway_dir), 0.0), 10.0);
         vec3 specular = spec * light_color;    
       
         // Calculate shadow                     
         vec3 lighting = (ambient + (1.0 - shadow) * (diffuse + specular)) * color; 
         lighting_sum = lighting_sum + lighting;
       }
       
       return vec4(lighting_sum / float(valid_lights_num), 1.0);
     } 
  )SHADER";
  mat.diffuse_map = std::make_shared<image3f32>(std::move(diffuse_map));
  return mat;
}
}