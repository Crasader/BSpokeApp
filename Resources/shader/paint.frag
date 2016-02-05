varying vec4 v_position;
varying vec2 v_texCoord;
varying vec4 v_fragmentColor;

void main(void)
{
    vec4 maskColor = texture2D(CC_Texture0, v_texCoord);
//    gl_FragColor.rgb = maskColor.rgb * maskColor.a + v_fragmentColor.rgb * maskColor.a;
    gl_FragColor = maskColor;
}
