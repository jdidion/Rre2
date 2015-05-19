#include <Rcpp.h>
#include "re2/re2.h"

class RegExpRE2;
RCPP_EXPOSED_CLASS_NODECL(RegExpRE2)

class MatchResultRE2;
RCPP_EXPOSED_CLASS_NODECL(MatchResultRE2)

// TODO: need to free memory

class MatchResultRE2 {
    public:
        MatchResultRE2(std::string text, std::vector<std::string> groups, int num_groups) {
            text_ = text;
            groups_ = groups;
            num_groups_ = num_groups;
        }

        std::string matched() { return text_; }
        int num_groups() { return num_groups_; }

        std::string group(int i) {
            if (i == 0) {
                return matched();
            }
            else if (i <= num_groups()) {
                return groups_[i-1];
            }
            else {
                throw std::range_error("Invalid group index " + std::to_string(i));
            }
        }

    private:
        std::string text_;
        std::vector<std::string> groups_;
        int num_groups_;
};

class RegExpRE2 {
public:
    RegExpRE2(std::string pattern) {
        // compile
        RE2::Options options;
        options.set_log_errors(false);
        RE2* regexp = new RE2(pattern, options);
        if (!regexp->ok()) {
            throw std::invalid_argument("Invalid pattern: " + pattern);
        }

        pattern_ = pattern;
        regexp_ = regexp;
        num_groups_ = regexp->NumberOfCapturingGroups();
    }

    MatchResultRE2 match(std::string text) {
        // Prepare the argument array to store the capturing groups
        const RE2::Arg* args[num_groups_];
        std::vector<std::string> match_vector;
        
        std::string* groups = new std::string[num_groups_];
        for (int i = 0; i < num_groups_; i++) {
            RE2::Arg arg = &groups[i];
            args[i] = &arg;
        }
        
        // Perform the match. Any extracted substrings will be
        // in args, and n will give the number of substrings.
        re2::StringPiece input(text);
        bool result = RE2::FullMatchN(input, *regexp_, args, num_groups_);
        if (result) {
            for (int i = 0; i < num_groups_; i++) {
                match_vector.push_back(groups[i]);
            }
            const MatchResultRE2 match_result(text, match_vector, num_groups_);
            return match_result;
        }
        else {
            throw std::invalid_argument("Pattern does not match string " + text);
        }
    }

private:
    std::string pattern_;
    RE2 *regexp_;
    int num_groups_;
};

RCPP_MODULE(re2) {
    using namespace Rcpp;
    
    class_<MatchResultRE2>("MatchResultRE2")

        .constructor<std::string, std::vector<std::string>, int>(
            "constructor with string text, string vector capturing groups, and int num groups")
        
        .method("matched",      &MatchResultRE2::matched,       "get matched string")
        .method("num_groups",   &MatchResultRE2::num_groups,    "get number of capturing groups")
        .method("group",        &MatchResultRE2::group,         "get group")
        ;
    
    class_<RegExpRE2>("RegExpRE2")

        .constructor<std::string>("constructor with string pattern")

        .method("match",        &RegExpRE2::match,         "perform match")
        ;
}
