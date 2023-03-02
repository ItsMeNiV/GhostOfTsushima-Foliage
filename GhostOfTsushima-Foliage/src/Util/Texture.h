#pragma once

class Texture
{
public:
	Texture(std::string&& path) : m_Path(path) {}

private:
	std::string m_Path;
};