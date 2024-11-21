#include <bits/stdc++.h>
#define ll long long
#define MAX_LINE 1024
using namespace std;


bool cmp(pair<string, int> a, pair <string, int> b) {
	return a.second < b.second;
}


vector <pair <string, int>> read_data() {
	FILE *file = fopen("record.csv", "r");
    if (!file) {
        perror("Could not open file");
    }

    char line[MAX_LINE];

	vector <pair <string, int>> v;
    while (fgets(line, MAX_LINE, file)) {
        char * token = strtok(line, ",");
		int i = 0;
		pair <string, string> p;
		int val;
        while (token) {
            if (i == 0) {
				p.first = token;
			} else {
				p.second = token;
				p.second[p.second.size() - 1] = '\0';
				val = stoi(p.second);
			}
			i++;
			token = strtok(NULL, ",");
        }
		v.push_back({p.first, val});
    }

	sort(v.begin(), v.end(), cmp);

    fclose(file);
	return v;
}

void write_data(const char * line) {
	FILE *file = fopen("record.csv", "a+");
    if (!file) {
        perror("Could not open file");
    }

    fprintf(file, line);
    fclose(file);
}

int main() {
	vector <pair <string, int>> v = read_data();

	for (auto pa: v) {
		cout << pa.first << "-" << pa.second << "\n";
	}

	const char * line = "Hello,69\n";
	write_data(line);

	v = read_data();

	for (auto pa: v) {
		cout << pa.first << "-" << pa.second << "\n";
	}
    return 0;
}
