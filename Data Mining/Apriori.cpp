#include "Apriori.h"

void Apriori::Setup(string filepath){
    string filename = "Horizontal_Format.xlsx - Sheet1.csv"; // relative path
    ifstream file(filename);

    if (!file.is_open())
    {
        cerr << "Could not open the file: " << filename << endl;
        return;
    }

    vector<vector<string>> data;
    string line;
    vector<Itemset> candidates;
    while (getline(file, line))
    {
        string tid, itemsStr;
        size_t commaPos = line.find(','); // first comma separates TiD
        tid = line.substr(0, commaPos);
        itemsStr = line.substr(commaPos + 1);

        // Remove quotes
        if (!itemsStr.empty() && itemsStr.front() == '"')
            itemsStr.erase(0, 1);
        if (!itemsStr.empty() && itemsStr.back() == '"')
            itemsStr.pop_back();

        stringstream itemStream(itemsStr);
        string item;
        vector<string> items;
        while (getline(itemStream, item, ','))
        {
            items.push_back(item);
            // assing id for each item
            if (itemIDMap[item] == 0)
            {
                Itemset newItem;
                newItem.id = id;
                newItem.item = item;
                candidates.push_back(newItem);
                idItemMap[id] = item;
                itemIDMap[item] = id++;
            }
        }

        cout << "TiD: " << tid << " Items: ";
        Transaction tr;
        for (auto& it : items)
        {
            tr.items[itemIDMap[it]] = 1;
            cout << itemIDMap[it] << " ";
        }
        Transactions.push_back(tr);
        cout << endl;
    }
    file.close();
    CandidateItemsets.push_back(candidates);
}

void Apriori::Calc_support(vector<int>v) {
	int lev = v.size();
	int sup = 0;
	for (int i = 0; i < Transactions.size(); i++)
	{
		int c = 0;
		for (int j = 0; j < lev; j++)
		{
			c += Transactions[i].items[v[j]];
		}
		if (c == lev)++sup;
	}
	if (sup >= minSupport) {
		Itemset item;
		item.ids = v;
		item.support = sup;
		if (FrequentItemsets.size() >= lev) {
			FrequentItemsets[lev - 1].push_back(item);
		}
		else {
			vector<Itemset>it;
			it.push_back(item);
			FrequentItemsets.push_back(it);
		}
	}
}

void Apriori::Join_step(int level) {
	vector<Itemset> newCandidates;
	for (int i = 0; i < FrequentItemsets[level - 1].size(); i++) {
		for (int j = i + 1; j < FrequentItemsets[level - 1].size(); j++) {
			bool canJoin = true;
			for (int k = 0; k < level - 1; k++) {
				if (FrequentItemsets[level - 1][i].ids[k] != FrequentItemsets[level - 1][j].ids[k]) {
					canJoin = false;
					break;
				}
			}
			if (canJoin) {
				Itemset joinedItem;
				joinedItem.ids = FrequentItemsets[level - 1][i].ids;
				joinedItem.ids.push_back(FrequentItemsets[level - 1][j].ids[level - 1]);
				joinedItem.id = id;
				id++;
				newCandidates.push_back(joinedItem);
			}
		}
	}
	CandidateItemsets.push_back(newCandidates);
}
