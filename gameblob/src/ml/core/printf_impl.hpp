// NOTE: This is a freestanding header in GCC, so this doesn't drag in the standard library.
#include <api/core/string.h>
#include <stdarg.h>

namespace ml {
	namespace dragons {

		class PrintfInput {
		   public:
			const static int Eof = -1;

			virtual bool eos() = 0;
			virtual int getc() = 0;
		};

		/// The output interface our printf impl consumes. This allows for two cases:
		/// - Unbuffered, slow, character-by-character output.
		/// - Fast all at once output
		class PrintfOutput {
		   public:
			virtual void putc(char c) = 0;
			virtual void puta(const char* pMem, u32 len) = 0;
		};

		enum PfFlags {
			Short = (1 << 0),
			Unsigned = (1 << 1),
			Hex = (1 << 2)
		};

		/// A abstracted printf implementation. This is shared across all our printf() like APIs,
		/// so printf functions just call one concrete implementation, saving code size.
		void vioprintf(PrintfInput& input, PrintfOutput& output, va_list val);

	} // namespace dragons
} // namespace ml
