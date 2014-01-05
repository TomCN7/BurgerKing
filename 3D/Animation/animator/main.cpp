#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "rayView.h"

#define DEFAULT_WIDTH 500
#define DEFAULT_HEIGHT 500
#define DEFAULT_COMPLEXITY 10
#define DEFAULT_DEBUG 0

static char *progname;

static void ShowUsage(void);


int main(int argc, char **argv){
	char *ray_filename = NULL;


	int width = DEFAULT_WIDTH, height = DEFAULT_HEIGHT; /* window parms */
	int complexity = DEFAULT_COMPLEXITY; 


	progname = argv[0];
	if (argc < 3) ShowUsage();
	argv++, argc--;
	while (argc > 0) {
		if (**argv == '-'){
			if (argc < 2) {
				fprintf(stderr, "Not enough arguments for %s\n", argv[0]);
				ShowUsage();
			}
			else if (!strcmp(*argv, "-help")) ShowUsage(); 
			else if (!strcmp(*argv, "-width")) width = atoi(argv[1]);
			else if (!strcmp(*argv, "-height")) height = atoi(argv[1]);
			else if (!strcmp(*argv, "-cplx")) complexity = atoi(argv[1]);
			else if (!strcmp(*argv, "-src"))  ray_filename = argv[1];
			else {
				fprintf(stderr, "%s: invalid option: %s\n", progname, *argv);
				ShowUsage();
			}
			argv++, argc--;
			argv++, argc--;
		} 
		else {
			fprintf(stderr, "%s: invalid option: %s\n", progname, *argv);
			ShowUsage();
		}
	}
	if (ray_filename==NULL) {
		fprintf(stderr, "%s: a ray (.ray) file must be defined!\n", progname);
		exit(EXIT_FAILURE);
	}
	RayView(ray_filename,width,height,complexity);
	return EXIT_SUCCESS;
}




static char options[] =
" \n"
"   -width <number of pixels>\n"
"   -height <number of pixels>\n"
"   -cplx <quadric tesselation (10-100)>\n"
"   -time <time to render ONLY one fixed frame at>\n"
"   -env <.ray file to use camera, lights and background from>\n"
"   -clk <clock rate in MHz> ** WINDOWS ONLY **\n"
"   -help\n"
" \n"
"Controls:\n"
" \n"
"  o Hold left mouse button and move mouse to rotate view about crystal ball center.\n"
"  o Hold middle mouse button and move mouse to zoom toward and away from crystal ball center.\n"
"  o Hold right mouse button to access menu.\n"
"  o Arrow keys move the crystal ball center up/down and left/right.\n"
"  o PageUp/PageDown move the crystal ball center forward and backward.\n"
"  o A/Z increase/decrease far clipping plane distance.\n"
"  o P dumps the camera position, direction and orientation to stderr.\n"
"  o T toggles the use of translation DOFS on and off.\n"
"  o C toggles display of a sphere at the crystall bal center (handy to get a feel for the interface)\n"
"  o ESC quits.\n"
"\n"
;



static void ShowUsage(void)
{
    fprintf(stderr, "\n\nUsage:\n\n %s [-option [ arg ] ...] -src <.ray file>\n",progname);
    fprintf(stderr, options);
    exit(EXIT_FAILURE);
}
