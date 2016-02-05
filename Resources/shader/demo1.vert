attribute vec4 a_position;
attribute vec2 a_texCoord;

varying vec4 v_position;
varying vec2 v_texCoord;

void main(void)
{
    gl_Position = CC_PMatrix * a_position;
    v_texCoord  = a_texCoord;
    v_position  = a_position;
}