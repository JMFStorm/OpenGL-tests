#include "free_type.hpp"
#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <iostream>
#include "main.hpp"
#include "j_util.hpp"
#include "shader.hpp"
#include "window.hpp"

void font_freetype_load(const char* fontPath, FreeTypeFont* font)
{
    FT_Library ftLib;
    FT_Face ftFace;
    FT_Error result;
    std::map<char, FreeTypeCharacter> characterArr;

    result = FT_Init_FreeType(&ftLib);
    j_assert(result == 0, "ERROR::FREETYPE: Could not init FreeType Library");
    result = FT_New_Face(ftLib, fontPath, 0, &ftFace);
    j_assert(result == 0, "ERROR::FREETYPE: Failed to load font");
    FT_Set_Pixel_Sizes(ftFace, 0, 24);
    glPixelStorei(GL_UNPACK_ALIGNMENT, 1);    // Disable byte-alignment restriction

    for (unsigned char c = 32; c < 128; c++) {
        result = FT_Load_Char(ftFace, c, FT_LOAD_RENDER);
        if (result != 0) {
            std::cout << "ERROR::FREETYTPE: Failed to load Glyph from unsigned char: " << c << std::endl;
            continue;
        }
        unsigned int texture;
        glGenTextures(1, &texture);
        glBindTexture(GL_TEXTURE_2D, texture);
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RED, ftFace->glyph->bitmap.width, ftFace->glyph->bitmap.rows, 0, GL_RED, GL_UNSIGNED_BYTE, ftFace->glyph->bitmap.buffer);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

        FreeTypeCharacter character = {
            texture,
            glm::ivec2(ftFace->glyph->bitmap.width, ftFace->glyph->bitmap.rows),
            glm::ivec2(ftFace->glyph->bitmap_left, ftFace->glyph->bitmap_top),
            ftFace->glyph->advance.x
        };
        characterArr.insert(std::pair<char, FreeTypeCharacter>(c, character));
    }

    FT_Done_Face(ftFace);
    FT_Done_FreeType(ftLib);
    unsigned int textVAO, textVBO;
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    glGenVertexArrays(1, &textVAO);
    glGenBuffers(1, &textVBO);
    glBindVertexArray(textVAO);
    glBindBuffer(GL_ARRAY_BUFFER, textVBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(float) * 6 * 4, NULL, GL_DYNAMIC_DRAW);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, 4 * sizeof(float), 0);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);

    unsigned int textShader = shader_create("./shaders/font_vertex_shader.shader", "./shaders/font_fragment_shader.shader");
    glm::mat4 projection = glm::ortho(0.0f, (float)window_width_default, 0.0f, (float)window_height_default);
    shader_use(textShader);
    shader_set_mat4(textShader, "projection", projection);
    shader_use(0);

    font->shader = textShader;
    font->vao = textVAO;
    font->vbo = textVBO;
    font->characters = characterArr;
}

void font_freetype_render(FreeTypeFont* font, std::string text, float screenX, float screenY, float scale, glm::vec3 color)
{
    shader_use(font->shader);
    shader_set_vec3(font->shader, "textColor", color.x, color.y, color.z);
    glActiveTexture(GL_TEXTURE0);
    glBindVertexArray(font->vao);

    std::string::const_iterator current;
    FreeTypeCharacter character;
    float xpos, ypos, cWidth, cHeight;
    const int indicies = 6;
    unsigned int drawCalls = 0;

    for (current = text.begin(); current != text.end(); current++) {
        character = font->characters[*current];
        xpos = screenX + character.bearing.x * scale;
        ypos = screenY - (character.size.y - character.bearing.y) * scale;
        cWidth = character.size.x * scale;
        cHeight = character.size.y * scale;
        float vertices[indicies * 4] = {
            xpos,           ypos + cHeight,   0.0f, 0.0f,
            xpos,           ypos,             0.0f, 1.0f,
            xpos + cWidth,  ypos,             1.0f, 1.0f,

            xpos,           ypos + cHeight,   0.0f, 0.0f,
            xpos + cWidth,  ypos,             1.0f, 1.0f,
            xpos + cWidth,  ypos + cHeight,   1.0f, 0.0f
        };
        glBindTexture(GL_TEXTURE_2D, character.textureId);
        glBindBuffer(GL_ARRAY_BUFFER, font->vbo);
        glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(vertices), vertices);
        glDrawArrays(GL_TRIANGLES, 0, indicies);
        drawCalls++;
        // Now advance cursors for next glyph (note that advance is number of 1/64 pixels)
        screenX += (character.advance >> 6) * scale; // bitshift by 6 to get value in pixels (2^6 = 64)
    }
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);
    glBindTexture(GL_TEXTURE_2D, 0);
}
