#include "Color.h"
bool Color::operator==(const Color& b) {
	return (red == b.red) && (green == b.green) && (blue == b.blue);
}