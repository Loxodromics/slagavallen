uniform highp sampler2D source;
uniform highp float qt_Opacity;
varying highp vec2 qt_TexCoord;

void main() {
	highp vec4 p = texture2D(source, qt_TexCoord);
	highp float a = qt_Opacity * p.a;
	gl_FragColor = vec4(p.rgb * a, a);
}
