using namespace std;
#include<iostream>
#include<stack>
#include<vector>
#include<string>

//定義全局變量colorlist，只用於輸出對應顏色，不參與算法實際的運算
vector<string> colorlist = { "深藍","淺藍","深綠","淺綠","深紫","淺紫","紅色","褐色","橙色","粉色" };
class Bottle {
public:
	stack<int> bottle;
public:
	Bottle(vector<int> num) {
		for (int i : num) {
			if (i == 0) break;
			bottle.push(i);
		}

	}
	~Bottle() {

	}
	int gettop() {
		if (getsize() == 0) return 0;
		return bottle.top();
	}
	void dopush(int num) {
		bottle.push(num);
	}
	void dopop() {
		bottle.pop();
	}
	int getsize() {
		return bottle.size();
	}
	bool iscomplete(){
		if (getsize() == 0) return true;
		if (getsize() < 4) return false;
		stack<int> temp = bottle;
		int color = temp.top();
		while (temp.size() > 0) {
			if (temp.top() != color) return false;
			temp.pop();
		}
		return true;
	}
	bool isonecolor() {
		if (getsize() == 0) return true;
		if (getsize() == 4) return false;
		stack<int> temp = bottle;
		int color = temp.top();
		while (temp.size() > 0) {
			if (temp.top() != color) return false;
			temp.pop();
		}
		return true;

	}
};
ostream& operator<<(ostream& cout, Bottle b) {
	stack<int> s = b.bottle;
	if (s.size() == 0) {
		cout << "空";
	}
	while (s.size() > 0) {
		cout << colorlist[s.top()-1]<<" ";
		s.pop();
	}
	return cout;
}
void initialize(int num, vector<Bottle> &bottles)
{
	//要動這裡的話，必須修改全局變量colorlist
	cout << "所有顏色：深藍、淺藍、深綠、淺綠、深紫、淺紫、紅色、褐色、橙色、粉色" << endl;
	cout << "對應編號：01、  02、  03、  04、  05、  06、  07、  08、  09、  10  " << endl;
	cout << "輸入每個試管的顏色，若空輸0" << endl;
	//初始化
	for (int i = 1; i <= num; i++) {
		vector<int> temp = {};
		cout << "輸入第" << i << "個試管的顏色編號（從下往上,空格隔開）： ";
		int j = 0;
		while (j < 4) {
			int temp_num = 0;
			cin >> temp_num;
			if (temp_num == 0) {
				temp.push_back(temp_num);
				break;
			}
			temp.push_back(temp_num);
			j++;
		}
		//可以增加一步輸入檢查:1.是否有四個數字，每個數字是否合法
		Bottle *bi = new Bottle(temp);
		bottles.push_back(*bi);
	}
	cout << "每個試管的顏色為（從上往下）：" << endl;
	int j = 1;
	for (auto i : bottles) {
		cout << "第" << j << "個試管的顏色： ";
		cout << i << endl;
		j++;
	}
}
vector<int> movecount(vector<Bottle> bottles, int num1,int num2) {
	//num1的試管移到num2
	int color = bottles[num1].gettop();
	vector<int> onestep = { -1,-1,-1,0 };
	if (bottles[num2].gettop() != color&& bottles[num2].getsize()!=0) return onestep;
	bottles[num1].dopop();
	onestep[0] = num1;
	onestep[1] = num2;
	onestep[2] = color;
	onestep[3]++;
	while (bottles[num1].gettop() == color&& bottles[num2].getsize()+ onestep[3]<=4) {
		onestep[3]++;
		bottles[num1].dopop();
	}
	return onestep;
}
void moveonestep(vector<Bottle> &bottles, vector<int> onestep) {
	for (int i = 0; i < onestep[3]; i++) {
		bottles[onestep[1]].dopush(onestep[2]);
		bottles[onestep[0]].dopop();
	}
}
void backonestep(vector<Bottle> &bottles, vector<int> onestep) {
	for (int i = 0; i < onestep[3]; i++) {
		bottles[onestep[0]].dopush(onestep[2]);
		bottles[onestep[1]].dopop();
	}
}
bool doneall(vector<Bottle> bottles) {
	for (auto i : bottles) {
		if (!i.iscomplete()) return false;
	}
	return true;
}
void solution(vector<Bottle> &bottles, stack<vector<int>> &solu,int num,int laststep) {
	if (doneall(bottles)) return;
	vector<int> onestep;//共4位，第一位記錄選中試管，第二位記錄目標試管，第三位記錄顏色編號，第四位記錄移動數目
	//從i管移到j管
	for (int i = 0; i < num;i++) {
		if (bottles[i].iscomplete()) continue;//i若已完成，則不能再移動
		if (i == laststep) continue;//上一次接受的管子，這次不能再移出
		for (int j = 0; j < num; j++) {
			if (i == j) continue;//不做自己對自己移動的操作
			if (bottles[j].getsize()==4) continue;//j管為4則不能接受
			if (bottles[j].getsize() == 0 && bottles[i].isonecolor()) continue;//純色不允許移到空管
			onestep = movecount(bottles, i, j);
			if (onestep[3] == 0) continue;//移動數目為0，跳過
			solu.push(onestep);
			moveonestep(bottles, onestep);
			if (doneall(bottles)) return;
			solution(bottles, solu,num,j);
			if (doneall(bottles)) return;
			backonestep(bottles, onestep);
			solu.pop();
		}
	}
}
int main(int argc, char *argv[])
{
	int num = 0;
	cout << "輸入總試管數：";
	cin >> num;
	vector<Bottle> bottles;
	initialize(num, bottles);
	stack<vector<int>> solu;
	solution(bottles, solu,num,-1);
	stack<vector<int>> solu2;
	while (solu.size()!=0) {
		solu2.push(solu.top());
		solu.pop();
	}
	while (solu2.size() != 0) {
		auto i = solu2.top();
		cout <<"從"<< i[0]+1 <<"移動到"<< i[1]+1 <<"，移動顏色為："<< colorlist[i[2]-1] <<"，移動數目為："<< i[3] << endl;
		solu2.pop();
	}
	cout << "最終結果，每個試管的顏色為（從上往下）：" << endl;
	int j = 1;
	for (auto i : bottles) {
		cout << "第" << j << "個試管的顏色： ";
		cout << i << endl;
		j++;
	}
	return 0;
}