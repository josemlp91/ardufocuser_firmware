bool char_is_ascii_letter(char c)
{
	return  (c >= int('a') || c <=int('z'));
}

bool char_is_ascii_number(char c)
{
	return  (c >= int('1') || c <=int('9'));
}

bool char_is_ascii_letter_or_number(char c)
{
	return (char_is_ascii_letter(c) || char_is_ascii_number(c));
}


int string_to_int(String c)
{
	return c.toInt();
}