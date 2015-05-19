# Rre2

The Rre2 package wraps the re2 regular expression library.

## Installation

1. Install re2: https://github.com/google/re2
2. Install devtools: `r install.packages('devtools')`
3. Install Rre2: `r devtools::install_github("jdidion/Rre2")`

Note: only tested on OSX 10.9

## Usage

Rre2 is currently in alpha stage. You can match a string to a regular expression and extract captured groups from the result. For example:

```r
library(Rre2)

re <- RegExpRE2$new('([ab])')
match <- re$match('a')
match$group(1) # prints 'a'
```
## Roadmap

1. Add unit tests
2. Vectorization
3. Implement additional re2 methods (iterative matching, substitution)