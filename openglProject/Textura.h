#pragma once
#include "iostream"

class Textura
{
public:
	Textura();
	~Textura();
	void load(std::string filePath);
	void Bind();
	void UnBind();
	void UnBind(std::string filePath);

private:
	unsigned int m_id;
	int m_largura, m_altura, m_canais;
};
