#include <bits/stdc++.h>
// #include <atcoder/all>
using namespace std;


// comment to disable debug functions
#define DEBUG

// frequent used aliases
typedef long long ll;
typedef pair<int, int> p;
typedef pair<ll, int> lp;
typedef pair<ll, ll> llp;
typedef vector<int> vec;
typedef vector<ll> vecll;
typedef vector<vec> mat;
typedef vector<vecll> matll;

// frequently used constants
const int INF  = 1e9+5;
const long long INFLL = 1e18+5;
const int NMAX = 105;
const int AMAX = 100005;
const int QMAX = 1005;
const int MOD  = 1e9 + 7;

// frequently used macros
#define FOR(i, N) for(int (i) = 0; (i) < (N); ++(i))
#define FOR2(i, j, N, M) for(int (i) = 0; (i) < (N); ++(i)) \
  for(int (j) = 0; (j) < (M); ++(j))

#if __cplusplus >= 201103L
#define ALL(v) begin(v),end(v)
#define SORT(v) sort(begin(v), end(v))
#define FIND(v,x) find(begin(v), end(v), (x))
#else
#define ALL(v) (v).begin(),(v).end()
#define SORT(v) sort(v.begin(), v.end())
#define FIND(v,x) find(v.begin(), v.end(), (x))
#endif

#define fi first
#define se second

#define MEMNEXT(from, to) do{ memmove((to), (from), sizeof(from)); \
memset((from), 0, sizeof(from)); } while(0)



//////////////////////////
// debug function begin //
//////////////////////////
#ifndef DEBUG

#define DUMP(x) do{}while(0)
void debug(const char* format, ...){}
#define DUMPV(v,c)

#else

#define DUMP(x) do{ std::cerr << (#x) << ": " << x << std::endl; }while(0)
void debug(const char* format, ...){
  va_list arg;
  va_start(arg, format);
  vprintf(format, arg);
  va_end(arg);
}
#define DUMPV(v, c) do{       \
  printf("%s: ", #v);         \
  for (int i = 0; i < (c); ++i) \
  {                           \
  cout << (v)[i] << " ";      \
  }                           \
  cout << endl;               \
} while(0)

#endif
////////////////////////
// debug function end //
////////////////////////


// std::fill of multi dimensions
template<typename A, size_t N, typename T>
void Fill(A (&array)[N], const T &val){
  std::fill( reinterpret_cast<T*>(array),
             reinterpret_cast<T*>(array+N),
             val );
}

// frequently used constants
static const int di[] = {-1, 0, 1, 0};
static const int dj[] = {0, 1, 0, -1};

////////////////////////////////////////////////////////////////////////////////////
///////// Main /////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////

// timer func
inline int get_time_millisec(void){
    return static_cast<ll>(chrono::duration_cast<chrono::milliseconds>(chrono::steady_clock::now().time_since_epoch()).count()) % INT_MAX;
}

struct Mountain {
  p pos;
  ll h;
};

typedef std::vector<Mountain> ans_t;

#define NUM_BRANCH 3
ll A[NMAX][NMAX],B[NMAX][NMAX];
ll N = 10, Q = 10, NOM = 10, ansScore = LLONG_MAX;
int threshold = 75; // 100分のいくつの確率で最小ロス近傍に遷移するか
ans_t branch[NUM_BRANCH];
ans_t ans;

ans_t generate_random(){
  ll tries = 10;
  auto ret = vector<Mountain>(Q);
  FOR(k, N) {
    int i = rand() % N;
    int j = rand() % N;
    int h = rand() % N;
    ret[k] = {{i, j}, h};
  }

  return ret;
}

void calc_B(ans_t ans_cand){
  Fill(B, 0);
  for (size_t i = 0; i < ans.size(); i++)
  {
    int y = ans[i].pos.fi;
    int x = ans[i].pos.se;
    int h = ans[i].h;
    for (size_t j = max(0, y - h); j < min<int>(N, y + h); j++)
    {
      for (size_t k = max(0, x - h); k < min<int>(N, x + h); k++)
      {
        B[j][k] += max<int>(0, h - abs<int>(j - y) - abs<int>(k - x));
      }
    }
  }
}

ll calc_diff(){
  ll diff = 0;
  FOR(i,N)
    FOR(j,N)
      diff += abs(A[i][j] - B[i][j]);
  return diff;
}

ll calcScore(ans_t ans_cand){
  calc_B(ans_cand);
  ll diff = calc_diff();
  debug("diff: %lld\n", diff);
  ll basePoints = 2e8 - diff;
  // TODO: calc bonus
  return basePoints;
}

void create_branch() {
  for (int i = 0; i < NUM_BRANCH; i++)
  {
    branch[i] = vector<Mountain>(Q);
    FOR(j,N) branch[i][j] = ans[j]; // とりあえずansをコピー

    for(int k = 0; k < NOM; ++k)
    {
      // 1こ選んで
      size_t chg_index = rand() % Q;
      // ガチランダムで適当に生成
      ll i_rand = rand() % 100;
      ll j_rand = rand() % 100;
      ll h_rand = rand() % 100;

      Mountain chg = Mountain{{i_rand, j_rand}, h_rand};
      branch[i][chg_index] = chg;
    }
  }
}

void select_branch() {
  auto maxAns = ans;
  ll maxScore = ansScore;
  for (int i = 0; i < NUM_BRANCH; i++)
  {
    ll candScore = calcScore(branch[i]);
    debug("candScore for branch[%d]: %lld\n", i, candScore);
    if (candScore > maxScore) {
      maxScore = candScore;
      maxAns = branch[i];
    }
  }
  if (rand() % 100 < threshold)
  {
    debug("Transitioned to new answer (Q: %d, score: %lld).\n",
      maxAns.size(), maxScore);
    ans = maxAns;
    ansScore = maxScore;
  }
  else
  {
    debug("Stayed at current answer (Q: %d, score: %lld).\n",
      ans.size(), ansScore);
  }
}

void update_NOM() {
  // TODO
  --NOM;
}

void debug(){
  // output debug information
  // FOR(j,N) { DUMPV(A[j], N); cout << endl; }
  cout << "ans: " << endl;
  for (int i = 0; i < ans.size(); i++)
  {
    printf("i: %d, X: %d, Y: %d, H: %lld\n",
      i, ans[i].pos.fi, ans[i].pos.se, ans[i].h);
  }
  cout << "B: " << endl;
  FOR(i, N) {
    FOR(j,N-1) printf("%3lld, ", B[i][j]);
    printf("%3lld\n", B[i][N-1]);
  }
}

void solve(){
  // Simulate Annealing
  int t = 0;
  int MAX_TIME = 5;

  while(t < MAX_TIME/* && diff > 0*/) {
    create_branch();
    select_branch();
    update_NOM();
    ++t;
  }
}

void answer(){
  // output answer

  // Uncomment if double
  // cout << fixed << setprecision(8);
  cout << ans.size() << endl;
  FOR(i, ans.size()) printf("%d %d %lld\n",
    ans[i].pos.first,
    ans[i].pos.second,
    ans[i].h);
}

void init(){
  // initialize for each test case
  // TODO: なんかrand()がうまく動いてないみたいだから他の乱数器を検討して
  std::srand(get_time_millisec()); //use current time as seed for random generator

  Fill(B, 0);
  ans = generate_random();
  ansScore = calcScore(ans);
}

void export_ans() {
  std::ostringstream stringStream;
  stringStream << "generated_n" << Q << "/" << rand() % 10000 << ".txt";
  std::string filename = stringStream.str();

  std::ofstream file;
  file.open(filename, std::ios::out);
  FOR(i,N) {
    FOR(j, N-1) file << B[i][j] << " ";
    file << B[i][N-1] << endl;
  }
  file.close();
  printf("Exported to %s.\n", filename.c_str());
}

int main(int argc, char const *argv[])
{
  init();

  if (argc > 1 && strcmp(argv[1], "--generate") == 0) {
    printf("GENERATE MODE: exporting randomly genarated board...\n");
    export_ans();
    return 0;
  }

  for(auto i = 0; i < N; ++i)
    for(auto j = 0; j < N; ++j)
      cin >> A[i][j];

  solve();

  #ifdef DEBUG
  debug();
  #endif

  answer();

  return 0;
}