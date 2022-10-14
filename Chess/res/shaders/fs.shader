#version 450 core

out vec4 FragColor;

in vec2 texPos;
in vec2 pixelPos;

uniform sampler2D tex0;
uniform vec4 color;
uniform int outChoice;
uniform int occupy;

void main()
{
	if (outChoice == 0 || outChoice == 1)
		FragColor = color;
	else if (outChoice == 2)
	{
		float distance = length(pixelPos);
		if (occupy == 0) //no one here, can move
		{//solid small circle
			if (distance < 0.3f)
				FragColor = vec4(0.0f, 0.0f, 0.0f, 0.1f); //select this
			else
				FragColor = vec4(0.0f, 0.0f, 0.0f, 0.0f);
		}

		else if (occupy == 2) //2 for enemy, 1 for comrade
		{//hollow big circle
			if (distance > 0.8f && distance < 0.95f)
				FragColor = vec4(0.0f, 0.0f, 0.0f, 0.1f);
			else
				FragColor = vec4(0.0f, 0.0f, 0.0f, 0.0f);
		}

	}
	else if (outChoice == 3)
		FragColor = texture(tex0, texPos);


};