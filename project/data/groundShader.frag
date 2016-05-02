$B_SHADER_VERSION
#ifdef GL_ES
precision lowp float;
#endif

varying vec4 colorVarying;

void main()
{

	gl_FragColor = vec4(0.99,0.54,0.09,1);
	
}

