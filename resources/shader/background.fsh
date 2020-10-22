uniform sampler2D texture;
uniform lowp float qt_Opacity;
uniform lowp vec4 color;

varying highp vec2 vTexCoord;

void main()
{
	lowp vec4 c = vec4(texture2D(texture, vTexCoord).rgb, color.w);
	gl_FragColor = c * qt_Opacity;
}
