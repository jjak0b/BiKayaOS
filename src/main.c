#include <scheduler.h>
#include <system/system.h>

int init( scheduler_t *scheduler ){
	int return_value = 0;
	return_value = scheduler_init();
	if( return_value ){
		/* stampa "main::init::scheduler_init() failed: returned: " return_value */
		return 1;
	}

	return return_value;
}

int main() {
	int return_value = 0;
	return_value = init( return_value );

	if( return_value ) {
		/* stampa "main::init() failed: returned: " return_value */
	}
	else {
		/* stampa "main::init() OK" */
	}

	
	if( !return_value ){
		return_value = scheduler_main( &scheduler );

		if( return_value ){
			/* stampa "main::scheduler_main() terminated: returned: " return_value */
		}
	}
	
	/* stampa "main() terminating: returned: " return_value */
	HALT();
}
