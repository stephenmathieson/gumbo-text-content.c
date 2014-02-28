
#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include "gumbo-parser/gumbo.h"
#include "gumbo-text-content.h"

#define EQUALS(a, b) ({                               \
  if (0 != strcmp(a, b))  {                           \
    fprintf(stderr                                    \
      , "Assertion error: \"%s\" == \"%s\" (%s:%d)\n" \
      , a                                             \
      , b                                             \
      , __FILE__                                      \
      , __LINE__);                                    \
    abort();                                          \
  }                                                   \
})

#define TEST(name) \
  static void test_##name(void)

#define RUN_TEST(test) \
  test_##test(); \
  puts("    \e[92m✓ \e[90m" #test "\e[0m");

TEST(single_node) {
  char *html = "<div>hello world</div>";
  GumboOutput *output = gumbo_parse(html);
  assert(output);

  char *text = gumbo_text_content(output->root);
  EQUALS("hello world", text);
  free(text);

  gumbo_destroy_output(&kGumboDefaultOptions, output);
}

TEST(wiki_page_li) {
  char *html =
    "<li>"
    "<a href=\"https://github.com/stephenmathieson/str-copy.c\">"
    "stephenmathieson/str-copy.c"
    "</a>"
    "- drop-in replacement for <code>strdup</code> with a "
    "less stupid name"
    "</li>";
  char *expected =
    "stephenmathieson/str-copy.c- "
    "drop-in replacement for strdup "
    "with a less stupid name";

  GumboOutput *output = gumbo_parse(html);
  assert(output);

  char *text = gumbo_text_content(output->root);
  EQUALS(expected, text);
  free(text);

  gumbo_destroy_output(&kGumboDefaultOptions, output);
}

TEST(wiki_page_li_with_link) {
  char *html =
    "<li>"
    "<a href=\"https://github.com/thlorenz/ee.c\">thlorenz/ee.c</a>"
    " - EventEmitter modeled after "
    "<a href=\"http://nodejs.org/api/events.html\">"
    "nodejs event emitter"
    "</a>"
    "</li>";
  char *expected =
    "thlorenz/ee.c - EventEmitter modeled after nodejs event emitter";
  GumboOutput *output = gumbo_parse(html);
  assert(output);

  char *text = gumbo_text_content(output->root);
  EQUALS(expected, text);
  free(text);

  gumbo_destroy_output(&kGumboDefaultOptions, output);
}

TEST(deeply_nested_text) {
  char *html =
    "<div>"
     "<div>"
      "<div>"
       "<div>"
        "<div>"
         "<div>"
          "<div>"
           "hello world"
          "</div>"
         "</div>"
        "</div>"
       "</div>"
      "</div>"
     "</div>"
    "</div>";
  char *expected = "hello world";
  GumboOutput *output = gumbo_parse(html);
  assert(output);

  char *text = gumbo_text_content(output->root);
  EQUALS(expected, text);
  free(text);

  gumbo_destroy_output(&kGumboDefaultOptions, output);
}

int
main(void) {
  clock_t start = clock();

  printf("\n  \e[36m%s\e[0m\n", "gumbo text content");
  RUN_TEST(single_node);
  RUN_TEST(wiki_page_li);
  RUN_TEST(wiki_page_li_with_link);
  RUN_TEST(deeply_nested_text);

  printf("\n");
  printf("  \e[90mcompleted in \e[32m%.5fs\e[0m\n"
    , (float) (clock() - start) / CLOCKS_PER_SEC);
  printf("\n");
}
