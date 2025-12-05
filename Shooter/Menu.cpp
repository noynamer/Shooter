#include "Menu.h"

AsMenu::AsMenu ()
{
	// загрузка шрифта
	font.loadFromFile("Font/BankGothic Md BT Medium.otf");

	// кнопка "Играть"
	createButton (play_button, L"Играть", 40, 200, 50, Color::White);
	// надпись для поля ввода
	createButton (input_label, L"Введите ширину экрана в пикселях", 24, 50, 150, Color::White);
	// текст в поле ввода
	createButton (input_text, L"", 24, 255, 205, Color::White);
	// кнопка "Enter"
	createButton (enter_button, L"Enter", 24, 470, 200, Color::White);
	// поле ввода
	createButton (input_box, Vector2f(200, 40), 250, 200, Color::Black, 2, Color::White);
}

void AsMenu::Draw(RenderWindow& win)
{
	win.clear(Color(50, 50, 50));

	win.draw(play_button);
	win.draw(input_label);
	win.draw(enter_button);
	win.draw(input_box);
	win.draw(input_text);

	win.display();
}

void AsMenu::setInputActive (bool active)
{
	input_active = active;
}

bool AsMenu::getInputActive () const
{
	return input_active;
}

void AsMenu::setInputString (const string& str)
{
	input_string = str;
}

string AsMenu::getInputString () const
{
	return input_string;
}

void AsMenu::setNumberValue (int value)
{
	number_value = value;
}

int AsMenu::getNumberValue () const
{
	return number_value;
}

void AsMenu::setInputBoxOutlineColor (Color color)
{
	input_box.setOutlineColor(color);
}

void AsMenu::setInputTextString (const string& str)
{
	input_text.setString(str);
}

bool AsMenu::isPlayButtonClicked (Vector2i mouse_pos) const
{
	return play_button.getGlobalBounds().contains(mouse_pos.x, mouse_pos.y);
}

bool AsMenu::isInputBoxClicked (Vector2i mouse_pos) const
{
	return input_box.getGlobalBounds().contains(mouse_pos.x, mouse_pos.y);
}

bool AsMenu::isEnterButtonClicked (Vector2i mouse_pos) const
{
	return enter_button.getGlobalBounds().contains(mouse_pos.x, mouse_pos.y);
}

void AsMenu::createButton (Text& name, const String& string, int size, int position_x, int position_y, Color color)
{
	name.setFont(font);
	name.setString(string);
	name.setCharacterSize(size);
	name.setPosition(position_x, position_y);
	name.setFillColor(color);
}

void AsMenu::createButton (RectangleShape& name, Vector2f size, int position_x, int position_y, Color color, int size_outline, Color color_outline)
{
	name.setSize(Vector2f(size.x , size.y));
	name.setPosition(position_x, position_y);
	name.setFillColor(color);
	name.setOutlineThickness(size_outline);
	name.setOutlineColor(color_outline);
}
