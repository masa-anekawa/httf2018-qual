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

// binary search
ll BSearch(ll ok, ll ng, bool (*f)(ll)){
  ll mid;
  while(abs(ok - ng) > 1LL) {
    mid =  (ok + ng) / 2LL;
    if(f(mid)) {
      debug("BSearch: f(%d) == true\n", mid);
      ok = mid;
    }
    else
    {
      debug("BSearch: f(%d) == false\n", mid);
      ng = mid;
    }
  }
  return ok;
}


// frequently used constants
static const int di[] = {-1, 0, 1, 0};
static const int dj[] = {0, 1, 0, -1};

static const int di8[] = {-1, 0, 1, -1, 1, -1, 0, 1};
static const int dj8[] = {-1, -1, -1, 0, 0, 1, 1, 1};

// frequently used structs
struct edge{
  int to,cost;
};

string YesNo(bool isYes) {
  return isYes ? "Yes" : "No";
}

ll NUM_TEST_CASE = 1;

////////////////////////////////////////////////////////////////////////////////////
///////// Main /////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////

ll N,M,K,A[NMAX][NMAX],B[NMAX][NMAX],C[NMAX][NMAX],D,E;

struct Mountain {
  p pos;
  ll h;
};

typedef std::vector<Mountain> ans_t;

#define NUM_BRANCH 3
ll Q = 100, NOM = 100;
ans_t branch[NUM_BRANCH];
ans_t ans;

void greedy(){
  // TODO
  ans = {{{0, 0}, 100}};
}
ll calc_diff(){
  ll diff = 0;
  FOR(i,N)
    FOR(j,N)
      diff += abs(A[i][j] - B[i][j]);
  return diff;
}
void create_branch() {
  for (int i = 0; i < NUM_BRANCH; i++)
  {
    branch[i] = vector<Mountain>(Q);
    FOR(j,N) branch[i][j] = ans[j]; // とりあえずansをコピー

    for(int j = 0; j < NOM; ++j)
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
  // TODO
  ans = branch[0];
}
void update_NOM() {
  // TODO
  --NOM;
}
ll calc_score(ans_t ans_cand){
  ll basePoints = 2e8 - calc_diff();
  // TODO: calc bonus
  return basePoints;
}
void calc_B(ans_t ans_cand){
  // TODO
  // for (size_t i = 0; i < ans.size(); i++)
  // {

  // }
  FOR(i,N) FOR(j,N) B[i][j] = rand() % AMAX;
}
void debug(){
  // output debug information
  // FOR(j,N) { DUMPV(A[j], N); cout << endl; }
  cout << "ans: " << endl;
  for (size_t i = 0; i < ans.size(); i++)
  {
    printf("i: %d, X: %d, Y: %d, H: %lld\n",
      i, ans[i].pos.fi, ans[i].pos.se, ans[i].h);
  }
}
void solve(){
  // まずいっこ
  NOM = 100;
  greedy();
  // Simulate Annealing
  // ll diff = calc_diff();
  ll score = INT_MAX;
  int t = 0;
  int MAX_TIME = 1;

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
void answerYesNo(){
  // output answer
  // cout << YesNo((bool)(ans)) << endl;
}
void init(){
  // initialize for each test case
  Fill(B, 0);
}
int main(int argc, char const *argv[])
{
  // operate inputs

  // cin >> NUM_TEST_CASE;

  for (int test_case = 0; test_case < NUM_TEST_CASE; ++test_case)
  {
    init();
    N = 100;
    for(auto i = 0; i < N; ++i)
      for(auto j = 0; j < N; ++j)
        cin >> A[i][j];
    solve();
    #ifdef DEBUG
    debug();
    #endif
    answer();
  }

  return 0;
}