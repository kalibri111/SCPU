#include "compiler/translator.h"
#include "soft_processor/proc.h"

int main() {
    translate("../listing_example.txt");
    __run__("../bitecode.txt");
}
