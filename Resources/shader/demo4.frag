uniform sampler2D u_tex2;

uniform vec2 u_offset1;
uniform vec2 u_offset2;
uniform vec2 u_offset3;
uniform vec2 u_offset4;

varying vec4 v_position;
varying vec2 v_texCoord;

void main()
{
	
	vec4 originColor = texture2D(CC_Texture0, v_texCoord.st);
    
    if(originColor.a == 0.0)
        discard;
    
    originColor.rgba = vec4(1.0, 1.0, 1.0, 1.0);
    
	vec4 color1 = texture2D(u_tex2, v_texCoord.st + u_offset1);
	vec4 color2 = texture2D(u_tex2, v_texCoord.st + u_offset2);
    vec4 color3 = texture2D(u_tex2, v_texCoord.st + u_offset3);
    vec4 color4 = texture2D(u_tex2, v_texCoord.st + u_offset4);

    vec4 finalColor = color1 + originColor * (1.0 - color1.a);
    finalColor = color2 + finalColor * (1.0 - color2.a);
    finalColor = color3 + finalColor * (1.0 - color3.a);
    finalColor = color4 + finalColor * (1.0 - color4.a);

    gl_FragColor.rgba = finalColor;
}
