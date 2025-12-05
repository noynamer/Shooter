#pragma once

#include <SFML/Graphics.hpp>
#include <string>
#include <iostream>

using namespace sf;
using namespace std;

class AsMenu
{
public:

	AsMenu();
	void Draw(RenderWindow& win);
	
	// геттеры и сеттеры
	void setInputActive(bool active);
	bool getInputActive() const;
	void setInputString(const string& str);
	string getInputString() const;
	void setNumberValue(int value);
	int getNumberValue() const;

	// методы для изменения внешнего вида элементов
	void setInputBoxOutlineColor(Color color);
	void setInputTextString(const string &str);

	// методы для проверки кликов
	bool isPlayButtonClicked(Vector2i mouse_pos) const;
	bool isInputBoxClicked(Vector2i mouse_pos) const;
	bool isEnterButtonClicked(Vector2i mouse_pos) const;


private:
	// создание кнопки
	void createButton(Text& name, const String& string, int size, int position_x, int position_y, Color color);
	void createButton(RectangleShape& name, Vector2f size, int position_x, int position_y, Color color, int size_outline, Color color_outline);

	Font font;
	Text play_button;
	Text input_label;
	Text input_text;
	Text enter_button;
	RectangleShape input_box;
	std::string input_string;
	int number_value = 0;
	bool input_active = false;
};

