#include <libpostal/libpostal.h>

#include <ruby.h>

#ifdef HAVE_RUBY_ENCODING_H
#include <ruby/encoding.h>
static rb_encoding *utf8Encoding;

#define ENCODED_STR_NEW2(str, encoding) \
  ({ \
   VALUE _string = rb_str_new2((const char *)str); \
   int _enc = rb_enc_get_index(encoding); \
   rb_enc_associate_index(_string, _enc); \
   _string; \
   })

#else

#define ENCODED_STR_NEW2(str, encoding) \
  rb_str_new2((const char *)str)

#endif

/* Older versions of Ruby (< 1.8.6) need these */
#ifndef RSTRING_PTR
#define RSTRING_PTR(s) (RSTRING(s)->ptr)
#endif
#ifndef RSTRING_LEN
#define RSTRING_LEN(s) (RSTRING(s)->len)
#endif
#ifndef RARRAY_PTR
#define RARRAY_PTR(s) (RARRAY(s)->ptr)
#endif
#ifndef RARRAY_LEN
#define RARRAY_LEN(s) (RARRAY(s)->len)
#endif

VALUE rb_parser = Qnil;

VALUE hash_get_symbol_or_string(VALUE hash, char *key) {
    VALUE value = rb_hash_aref(hash, rb_str_new2(key));

    if (value != Qnil) {
        return value;
    }

    return rb_hash_aref(hash, ID2SYM(rb_intern(key)));
}


VALUE rb_parse_address(int argc, VALUE *argv, VALUE self) {
    if (argc < 2) {  // there should only be 1 or 2 arguments
        rb_raise(rb_eArgError, "Usage: parse_address(address, options = {})");
        return Qnil;
    }

    VALUE input;
    VALUE rb_options;

    rb_scan_args(argc, argv, "2", &input, &rb_options);

    if (TYPE(input) != T_STRING) {
        rb_raise(rb_eArgError, "input must be a string");
        return Qnil;
    }

    if (TYPE(rb_options) != T_HASH) {
        rb_raise(rb_eArgError, "options must be a hash");
        return Qnil;
    }

    VALUE rb_language = hash_get_symbol_or_string(rb_options, "language");
    VALUE rb_country = hash_get_symbol_or_string(rb_options, "country");

    char *address = RSTRING_PTR(input);

    libpostal_address_parser_response_t *parsed;
    libpostal_address_parser_options_t options = libpostal_get_address_parser_default_options();
    if (rb_language != Qnil) {
        options.language = RSTRING_PTR(rb_language);
    }

    if (rb_country != Qnil) {
        options.country = RSTRING_PTR(rb_country);
    }

    if ((parsed = libpostal_parse_address(address, options))) {
        size_t n = parsed->num_components;
        VALUE rb_parse_result = rb_ary_new2(n);

        for (size_t i = 0; i < n; i++) {
            VALUE rb_component = rb_ary_new2(2);
            rb_ary_store(rb_component, 0, rb_str_new2(parsed->components[i]));
            rb_ary_store(rb_component, 1, ID2SYM(rb_intern(parsed->labels[i])));

            rb_ary_store(rb_parse_result, i, rb_component);
        }

        libpostal_address_parser_response_destroy(parsed);

        return rb_parse_result;
    }

    return Qnil;

}

void Init_parser() {
    if (!libpostal_setup() || !libpostal_setup_parser()) {
        rb_raise(rb_eLoadError, "Could not load libpostal");
        return;
    }

    rb_parser = rb_define_module("CParser");

    rb_define_module_function(rb_parser, "parse_address", rb_parse_address, -1);
}

