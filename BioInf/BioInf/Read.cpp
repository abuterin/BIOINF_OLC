#include "Read.hpp"

string Read::reverseComplement() {
	if (_reverseComplement.empty()) {
		for (int i = _read.length() - 1; i >= 0; i--) {
			if (_read[i] == 'A') {
				_reverseComplement.append("T");
			}
			else if (_read[i] == 'T') {
				_reverseComplement.append("A");
			}
			else if (_read[i] == 'G') {
				_reverseComplement.append("C");
			}
			else if (_read[i] == 'C') {
				_reverseComplement.append("G");
			}
		}
	}
	return _reverseComplement;
}
