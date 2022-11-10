#version 450 core

out vec4 FragColor;

in vec2 texPos;
in vec2 pixelPos;

uniform sampler2D tex;
uniform vec4 color;
uniform int outChoice;
uniform int occupy;

void main()
{
	if (outChoice == 0 || outChoice == 1) //for board(0) and yellow(1)
		FragColor = color;
	else if (outChoice == 2) //move previews
	{
		float distance = length(pixelPos);
		if (occupy == 0) //no one here, can move
		{
			if (distance < 0.3f) //solid small circle
				FragColor = vec4(0.0f, 0.0f, 0.0f, 0.1f);
			else
				FragColor = vec4(0.0f, 0.0f, 0.0f, 0.0f);
		}

		else if (occupy == 2) //2 for enemy, 1 for comrade
		{
			if (distance > 0.8f && distance < 0.95f) //hollow big circle
				FragColor = vec4(0.0f, 0.0f, 0.0f, 0.1f);
			else
				FragColor = vec4(0.0f, 0.0f, 0.0f, 0.0f);
		}

	}
	else if (outChoice == 3) //piece 
		FragColor = texture(tex, texPos);
};