# vCard c++ simple library

This is a C++ library for parsing, writing and manipulating vCards.
Inspired by libvcard 1.0.0 and ez-vcard

### Code exmaples

Reading text .vcf file:

```c++
#include "vcard.h"
#include "text_io.h"
...
std::ifstream ifs;
ifs.open("test.vcf");
TextReader tr(ifs);
std::vector<vCard> cards = tr.parseCards();

```

Creating vCard objects and write it to string buffer (or it can be file stream):

```c++
vCardParamMap params;
params.addParam("TYPE", "home");
params.addParam("TYPE", "work");
params.addParam("LANGUAGE", "en");

vCardProperty p = vCardProperty::createAddress("Street", "City", "Nevada", "112233", "USA");
p << params;

vCard card;
card << p;

std::stringstream s;
TextWriter tw(s);
tw << card;
```

More writers and readers will be added soon (for xCards and jCards formats)

For more examples check test/tests.cpp file