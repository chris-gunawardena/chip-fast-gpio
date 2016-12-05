#include <node.h>
#include <v8.h>
using namespace v8;


#if __STDC_VERSION__ >= 199901L
#define _XOPEN_SOURCE 600
#else
#define _XOPEN_SOURCE 500
#endif /* __STDC_VERSION__ */

#include <fcntl.h>
#include <sys/types.h>
#include <sys/uio.h>
#include <stdio.h>
#include <time.h>
#include <unistd.h>
#include <errno.h>
#include <string.h>
#include <stdlib.h>

/* See http://wiki.geeky-boy.com/w/index.php?title=Internal_error_handling */
#define WCHK(cond_expr) do { \
  if (!(cond_expr)) { \
    fprintf(stderr, "%s:%d, Warning, expected '%s' to be true (%s)\n", \
      __FILE__, __LINE__, #cond_expr, strerror(errno)); \
  }  \
} while (0)

#define ECHK(cond_expr) do { \
  if (!(cond_expr)) { \
    fprintf(stderr, "%s:%d, Error, expected '%s' to be true (%s)\n", \
      __FILE__, __LINE__, #cond_expr, strerror(errno)); \
    abort(); \
  }  \
} while (0)

const char* ToCString(const String::Utf8Value& value) {
  return *value ? *value : "<string conversion failed>";
}

const char* join(const char* str1, const char* str2, const char* str3) {
  char* joint_str = (char*) malloc(strlen(str1) + strlen(str2) + strlen(str3) + 1);
  strcpy(joint_str, str1);
  strcat(joint_str, str2);
  strcat(joint_str, str3);
  return joint_str;
}

void GPIOwrite(const v8::FunctionCallbackInfo<v8::Value>& args)
{
	v8::Isolate* isolate = args.GetIsolate();
	v8::HandleScope scope(isolate);

    v8::String::Utf8Value pinUtf8(args[0]->ToString());
	const char* pin = ToCString(pinUtf8);
	//printf("pin: %s\n", pin);

    v8::String::Utf8Value valueUtf8(args[1]->ToString());
	const char* value = ToCString(valueUtf8);
	//printf("value: %s\n", value);


	// export pin
	int export_fd = open("/sys/class/gpio/export", O_WRONLY);  ECHK(export_fd != -1);
	/* If port 5 is already exported, the following will fail. That's OK, but warn. */
	int wlen = write(export_fd, pin, 4);  WCHK(wlen == 4);
	close(export_fd);	

    // set for output
    //printf(join("/sys/class/gpio/gpio", pin, "/direction"));
	int dir_fd = open(join("/sys/class/gpio/gpio", pin, "/direction"), O_RDWR);  ECHK(dir_fd != -1);
	wlen = write(dir_fd, "out", 3);  ECHK(wlen == 3);
	close(dir_fd);

	// // write
    int out_val_fd = open(join("/sys/class/gpio/gpio", pin, "/value"), O_RDWR);  ECHK(out_val_fd != -1);
	wlen = write(out_val_fd, value, 2);  ECHK(wlen == 2);
	close(out_val_fd);

	// unexport
	export_fd = open("/sys/class/gpio/unexport", O_WRONLY);  ECHK(export_fd != -1);
	wlen = write(export_fd, pin, 4);  ECHK(wlen == 4);
	close(export_fd);
}


void init(v8::Local<v8::Object> exports) {
  NODE_SET_METHOD(exports, "write", GPIOwrite);
}

NODE_MODULE(binding, init);