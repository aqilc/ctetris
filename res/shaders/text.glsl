#version 330 core
layout (location = 0) in vec4 pos;
layout (location = 1) in vec2 txt;

uniform mat4 mvp;

out vec2 text;

void main()
{
    gl_Position = mvp * pos;
    text = txt;
}

# frag
#version 330 core
layout (location = 0) out vec4 color;

in vec2 text;

uniform sampler2D tex;

void main()
{
    color = vec4(1.0, 1.0, 1.0, texture(tex, text).r);
    // color = vec4(textColor, 1.0) * sampled;
}
