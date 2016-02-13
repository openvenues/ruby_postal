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

VALUE rb_expand = Qnil;


VALUE hash_get_symbol_or_string(VALUE hash, char *key) {
    VALUE value = rb_hash_aref(hash, rb_str_new2(key));

    if (value != Qnil) {
        return value;
    }

    return rb_hash_aref(hash, ID2SYM(rb_intern(key)));
}


VALUE rb_expand_address(int argc, VALUE *argv, VALUE self) {
    if (argc < 2) {  // there should only be 1 or 2 arguments
        rb_raise(rb_eArgError, "Usage: expand_address(address, options = {})");
        return Qnil;
    }

    VALUE input;
    VALUE rb_options;

    rb_scan_args(argc, argv, "2", &input, &rb_options);

    if (TYPE(rb_options) != T_HASH) {
        rb_raise(rb_eArgError, "options must be a hash");
        return Qnil;
    }

    VALUE rb_languages = hash_get_symbol_or_string(rb_options, "languages");
    if (rb_languages != Qnil && (TYPE(rb_languages) != T_ARRAY || RARRAY_LEN(rb_languages) == 0)) {
        rb_raise(rb_eArgError, "options['languages'] must be a non-empty array");
        return Qnil;
    }

    size_t len_languages = 0;

    if (rb_languages != Qnil) {
        len_languages = (size_t)RARRAY_LEN(rb_languages);
    }

    normalize_options_t options = LIBPOSTAL_DEFAULT_OPTIONS;

    size_t i;
    char **languages = NULL;
    size_t num_languages = 0;

    if (len_languages > 0) {
        languages = malloc(sizeof(char *) * len_languages);

        for (i = 0; i < len_languages; i++) {
            VALUE rb_lang = rb_ary_entry(rb_languages, (int)i);
            if (rb_lang != Qnil && TYPE(rb_lang) == T_STRING) {
                size_t rb_lang_len = RSTRING_LEN(rb_lang);
                if (rb_lang_len > 0 && rb_lang_len < MAX_LANGUAGE_LEN) {
                    char *lang = RSTRING_PTR(rb_lang);
                    languages[num_languages++] = lang;
                }
            }
        }
    }

    options.num_languages = num_languages;
    options.languages = languages;

    char *address = RSTRING_PTR(input);

    VALUE address_components = hash_get_symbol_or_string(rb_options, "languages");
    if (address_components != Qnil && TYPE(address_components) == T_FIXNUM) {
        options.address_components = (uint16_t) NUM2UINT(address_components);
    }

    VALUE latin_ascii = hash_get_symbol_or_string(rb_options, "latin_ascii");
    if (latin_ascii != Qnil) {
        options.latin_ascii = RTEST(latin_ascii);
    }

    VALUE transliterate = hash_get_symbol_or_string(rb_options, "transliterate");
    if (transliterate != Qnil) {
        options.transliterate = RTEST(transliterate);
    }

    VALUE strip_accents = hash_get_symbol_or_string(rb_options, "strip_accents");
    if (strip_accents != Qnil) {
        options.strip_accents = RTEST(strip_accents);
    }

    VALUE decompose = hash_get_symbol_or_string(rb_options, "decompose");
    if (decompose != Qnil) {
        options.decompose = RTEST(decompose);
    }

    VALUE lowercase = hash_get_symbol_or_string(rb_options, "lowercase");
    if (lowercase != Qnil) {
        options.lowercase = RTEST(lowercase);
    }

    VALUE trim_string = hash_get_symbol_or_string(rb_options, "trim_string");
    if (trim_string != Qnil) {
        options.trim_string = RTEST(trim_string);
    }

    VALUE drop_parentheticals = hash_get_symbol_or_string(rb_options, "drop_parentheticals");
    if (drop_parentheticals != Qnil) {
        options.drop_parentheticals = RTEST(drop_parentheticals);
    }

    VALUE replace_numeric_hyphens = hash_get_symbol_or_string(rb_options, "replace_numeric_hyphens");
    if (replace_numeric_hyphens != Qnil) {
        options.replace_numeric_hyphens = RTEST(replace_numeric_hyphens);
    }

    VALUE delete_numeric_hyphens = hash_get_symbol_or_string(rb_options, "delete_numeric_hyphens");
    if (delete_numeric_hyphens != Qnil) {
        options.delete_numeric_hyphens = RTEST(delete_numeric_hyphens);
    }

    VALUE split_alpha_from_numeric = hash_get_symbol_or_string(rb_options, "split_alpha_from_numeric");
    if (split_alpha_from_numeric != Qnil) {
        options.split_alpha_from_numeric = RTEST(split_alpha_from_numeric);
    }

    VALUE replace_word_hyphens = hash_get_symbol_or_string(rb_options, "replace_word_hyphens");
    if (replace_word_hyphens != Qnil) {
        options.replace_word_hyphens = RTEST(replace_word_hyphens);
    }

    VALUE delete_word_hyphens = hash_get_symbol_or_string(rb_options, "delete_word_hyphens");
    if (delete_word_hyphens != Qnil) {
        options.delete_word_hyphens = RTEST(delete_word_hyphens);
    }

    VALUE delete_final_periods = hash_get_symbol_or_string(rb_options, "delete_final_periods");
    if (delete_final_periods != Qnil) {
        options.delete_final_periods = RTEST(delete_final_periods);
    }


    VALUE delete_acronym_periods = hash_get_symbol_or_string(rb_options, "delete_acronym_periods");
    if (delete_acronym_periods != Qnil) {
        options.delete_acronym_periods = RTEST(delete_acronym_periods);
    }

    VALUE drop_english_possessives = hash_get_symbol_or_string(rb_options, "drop_english_possessives");
    if (drop_english_possessives != Qnil) {
        options.drop_english_possessives = RTEST(drop_english_possessives);
    }

    VALUE delete_apostrophes = hash_get_symbol_or_string(rb_options, "delete_apostrophes");
    if (delete_apostrophes != Qnil) {
        options.delete_apostrophes = RTEST(delete_apostrophes);
    }

    VALUE expand_numex = hash_get_symbol_or_string(rb_options, "expand_numex");
    if (expand_numex != Qnil) {
        options.expand_numex = RTEST(expand_numex);
    }

    VALUE roman_numerals = hash_get_symbol_or_string(rb_options, "roman_numerals");
    if (roman_numerals != Qnil) {
        options.roman_numerals = RTEST(roman_numerals);
    }

    size_t num_expansions = 0;
    char **expansions = expand_address(address, options, &num_expansions);

    VALUE rb_expansions = rb_ary_new2(num_expansions);
    for (size_t i = 0; i < num_expansions; i++) {
        char *expansion = expansions[i];
        VALUE rb_expansion = rb_str_new2(expansion);
        rb_ary_store(rb_expansions, i, rb_expansion);
        free(expansion);
    }
    free(expansions);

    return rb_expansions;
}

void Init_expand() {
    if (!libpostal_setup() || !libpostal_setup_language_classifier()) {
        rb_raise(rb_eLoadError, "Could not load libpostal");
        return;
    }

    rb_expand = rb_define_module("CExpand");

    rb_define_module_function(rb_expand, "expand_address", rb_expand_address, -1);

    rb_define_global_const("ADDRESS_NONE", UINT2NUM(ADDRESS_NONE));
    rb_define_global_const("ADDRESS_ANY", UINT2NUM(ADDRESS_ANY));
    rb_define_global_const("ADDRESS_NAME", UINT2NUM(ADDRESS_NAME));
    rb_define_global_const("ADDRESS_HOUSE_NUMBER", UINT2NUM(ADDRESS_HOUSE_NUMBER));
    rb_define_global_const("ADDRESS_STREET", UINT2NUM(ADDRESS_STREET));
    rb_define_global_const("ADDRESS_UNIT", UINT2NUM(ADDRESS_UNIT));
    rb_define_global_const("ADDRESS_LOCALITY", UINT2NUM(ADDRESS_LOCALITY));
    rb_define_global_const("ADDRESS_ADMIN1", UINT2NUM(ADDRESS_ADMIN1));
    rb_define_global_const("ADDRESS_ADMIN2", UINT2NUM(ADDRESS_ADMIN2));
    rb_define_global_const("ADDRESS_ADMIN3", UINT2NUM(ADDRESS_ADMIN3));
    rb_define_global_const("ADDRESS_ADMIN4", UINT2NUM(ADDRESS_ADMIN4));
    rb_define_global_const("ADDRESS_ADMIN_OTHER", UINT2NUM(ADDRESS_ADMIN_OTHER));
    rb_define_global_const("ADDRESS_POSTAL_CODE", UINT2NUM(ADDRESS_POSTAL_CODE));
    rb_define_global_const("ADDRESS_NEIGHBORHOOD", UINT2NUM(ADDRESS_NEIGHBORHOOD));
    rb_define_global_const("ADDRESS_ALL", UINT2NUM(ADDRESS_ALL));
}

