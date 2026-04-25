
/// A timer bar
class uiProgressBar {
	float progress;

	u32 x;
	u32 y;
	u32 width;
	u32 height;

	// FIXME: we need colours!!!!!!!
	// tARGB foreColor
	// tARGB barColor
   public:
	uiProgressBar(u32 x, u32 y, u32 width, u32 height);

	/// Sets the new progress value.
	void setProgress(float progress);

	/// Call to.. well, render the bar.
	void render();
};
