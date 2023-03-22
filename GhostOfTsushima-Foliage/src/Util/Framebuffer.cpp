#include "pch.h"
#include "Framebuffer.h"

Framebuffer::Framebuffer(int width, int height, int sampleCount)
    : m_CurrentWidth(width), m_CurrentHeight(height), m_RenderBuffer(UINT_MAX)
{
    glGenFramebuffers(1, &m_FrameBuffer);
    glBindFramebuffer(GL_FRAMEBUFFER, m_FrameBuffer);
    m_TextureColorBuffer = CreateRef<Texture>(width, height, sampleCount);

    glGenRenderbuffers(1, &m_RenderBuffer);
    glBindRenderbuffer(GL_RENDERBUFFER, m_RenderBuffer);
    if (sampleCount > 1)
        glRenderbufferStorageMultisample(GL_RENDERBUFFER, sampleCount, GL_DEPTH24_STENCIL8, width, height);
    else
        glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH24_STENCIL8, width, height);
    glBindRenderbuffer(GL_RENDERBUFFER, 0);

    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, m_TextureColorBuffer->GetTextureType(), m_TextureColorBuffer->GetTextureId(), 0);
    glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_RENDERBUFFER, m_RenderBuffer);

    if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
    {
        std::cout << "ERROR::FRAMEBUFFER:: Framebuffer is not complete!" << std::endl;
        std::cout << std::to_string(glCheckFramebufferStatus(GL_FRAMEBUFFER)) << std::endl;
    }
    glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

Framebuffer::~Framebuffer()
{
    glDeleteRenderbuffers(1, &m_RenderBuffer);
    glDeleteFramebuffers(1, &m_FrameBuffer);
}

void Framebuffer::Bind()
{
    glBindFramebuffer(GL_FRAMEBUFFER, m_FrameBuffer);
}

void Framebuffer::Unbind()
{
    glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

void Framebuffer::BlitFramebuffer(unsigned int targetFramebuffer)
{
    glBindFramebuffer(GL_READ_FRAMEBUFFER, m_FrameBuffer);
    glBindFramebuffer(GL_DRAW_FRAMEBUFFER, targetFramebuffer);
    glBlitFramebuffer(0, 0, m_CurrentWidth, m_CurrentHeight, 0, 0, m_CurrentWidth, m_CurrentHeight, GL_COLOR_BUFFER_BIT, GL_NEAREST);
    glBindFramebuffer(GL_READ_FRAMEBUFFER, 0);
    glBindFramebuffer(GL_DRAW_FRAMEBUFFER, 0);
}