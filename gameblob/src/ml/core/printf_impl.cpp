#include "printf_impl.hpp"

namespace ml {
	namespace dragons {

		enum PrintfState {
			Printf_Echo,
			Printf_Formatting,
		};

		void vioprintf(PrintfInput& formatInput, PrintfOutput& output, va_list val) {
			PrintfState state = Printf_Echo;
			int width = -1;

			while(!formatInput.eos()) {
				int c = formatInput.getc();
				if(c == PrintfInput::Eof)
					break;

				switch(state) {
					case Printf_Echo:
						if(c == '%') {
							state = Printf_Formatting;
						} else {
							output.putc(c);
						}
						break;

					case Printf_Formatting: {
						switch(c) {
							case '0' ... '9':
								width = (int)(c - '0');
								break;

							case 'x':
								// Assume 8 digits, if width was unitialized
								if(width == -1)
									width = 8;

								// Re-uninitalize the width.
								width = -1;
								state = Printf_Echo;
								break;

							case 'c':
								output.putc(va_arg(val, char));
								state = Printf_Echo;
								break;

							case 's': {
								char* pString = va_arg(val, char*);
								if(pString == nil(char*)) {
									output.puta("(null)", sizeof("(null)") - 1);
								} else {
									output.puta(pString, mlStrLen(pString));
								}
								state = Printf_Echo;
							} break;

							case 'd': {
								int a = va_arg(val, int);

								state = Printf_Echo;
							};

							case '%':
								output.putc('%');
								state = Printf_Echo;
								break;
						}
					} break;
				}
			}
		}

	} // namespace dragons
} // namespace ml
