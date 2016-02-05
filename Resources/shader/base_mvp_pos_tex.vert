attribute vec4 a_position;
attribute vec2 a_texCoord;
attribute vec4 a_color;

varying vec4 v_position;
varying vec2 v_texCoord;
varying vec4 v_fragmentColor;

void main(void)
{
    gl_Position = CC_MVPMatrix * a_position;
    v_texCoord  = a_texCoord;
    v_position  = a_position;
    v_fragmentColor = a_color;
}