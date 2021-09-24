#include <bits/stdc++.h>
// #include <atcoder/all>
using namespace std;


// 重要: 本番時にはコメントアウトする
#define DEBUG



/////////////////////////////////////////////////////////////////////////////////////
///////// 読まなくていいゾーン① ここから ////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////


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
// const int INF  = 1e9+5;
// const long long INFLL = 1e18+5;
// const int MOD  = 1e9 + 7;

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
// static const int di[] = {-1, 0, 1, 0};
// static const int dj[] = {0, 1, 0, -1};

// timer func
inline int get_time_millisec(void){
  return static_cast<ll>(
    chrono::duration_cast<chrono::milliseconds>(
      chrono::steady_clock::now().time_since_epoch()
    ).count()
  ) % INT_MAX;
}


/////////////////////////////////////////////////////////////////////////////////////
///////// 読まなくていいゾーン① ここまで ////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////



/**
 * 1回の山型足し算をあらわす構造体
 */
struct Mountain {
  /// (Y, X)の順に格納(== (i, j) の順に格納)
  p pos;
  ll h;
};

typedef std::vector<Mountain> ans_t; // 解ベクトルの型


const int BASE_POINT_MAX = 2e8;
const int N_MAX = 105;   // 盤面幅・高さの最大値(少し大きめに)
// const int A_MAX = 100005;
// const int Q_MAX = 1005;
const int NUM_BRANCH = 3; // 近傍探索数
const int MAX_TIME = 50;   // 探索回数の上限

/**
 * 1つのブランチにQ回の変異を起こさせるような、diffの値。
 * <code>ansScore</code> がこれより高ければ、Q回以上の変異が起きる。
 * 反対に <code>ansScore</code> がこれより低ければ、変異はQ回より減る。
 * diffが0の場合、変異は起こらなくなる。
 */
const ll BASE_DIFF = 500;

int NOM = 10;       // Number of Modifications(1つの解ベクトルに対するランダムな変異の回数)
int annealing_time = 0;
ll A[N_MAX][N_MAX];      // 入力
ll B[N_MAX][N_MAX];      // 解候補ベクトルから生成した盤面(使いまわす)
ll N = 10;               // 盤面の幅(=高さ)
ll Q = 10;               // 解ベクトルの長さ
ll ansScore = LLONG_MAX; // 解候補のスコア
double threshold = 0.8; // よりスコアが高くなる近傍があるとき、その最大スコア近傍に遷移する確率

ans_t branch[NUM_BRANCH]; // 近傍
ans_t ans; // 解ベクトル

std::function<double()> randomDouble; // [0, 1) の範囲で乱数を返す関数



//////////////////////////////////////////////////////////////////////////////////////
///////// 読まなくていいゾーン② ここから /////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////

/// ランダム関数の初期化
void initRandomFunction() {
  std::random_device randomDevice;
  std::vector<uint32_t> randomSeedVector(10);
  std::generate(randomSeedVector.begin(), randomSeedVector.end(), std::ref(randomDevice));
  std::seed_seq randomSeed(randomSeedVector.begin(), randomSeedVector.end());

  std::mt19937 randomEngine(randomSeed);

  randomDouble = std::bind(std::uniform_real_distribution<>(), randomEngine);
}

/// デバッグ用関数
void DUMP_ANS(ans_t a) {
  #ifdef DEBUG
  debug("[");
  FOR(h, Q-1) debug("{{%d, %d}, %d}, ", a[h].pos.fi, a[h].pos.se, a[h].h);
  debug("{{%d, %d}, %d}]\n", a[Q-1].pos.fi, a[Q-1].pos.se, a[Q-1].h);
  #endif
}

/// デバッグ用関数
void DUMP_A() {
  #ifdef DEBUG
  for (int i = 0; i < N; i++)
  {
    cout << "\t";
    for (int j = 0; j < N; j++)
    {
      debug("%3lld ", A[i][j]);
    }
    cout << endl;
  }
  cout << endl;
  #endif
}

/// デバッグ用関数
void DUMP_B() {
  #ifdef DEBUG
  for (int i = 0; i < N; i++)
  {
    cout << "\t";
    for (int j = 0; j < N; j++)
    {
      debug("%3lld ", B[i][j]);
    }
    cout << endl;
  }
  cout << endl;
  #endif
}

/// デバッグ用関数
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

//////////////////////////////////////////////////////////////////////////////////////
///////// 読まなくていいゾーン② ここまで /////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////


/// 初期解を生成する
ans_t generateRandomAnswer(){
  auto ret = vector<Mountain>(Q);
  FOR(k, Q) {
    int i = randomDouble() * N;
    int j = randomDouble() * N;
    int h = randomDouble() * N + 1; // 1 <= H <= N
    ret[k] = {{i, j}, h};
  }

  debug("Finished generateRandomAnswer(). Now initial answer is:\n\t");
  DUMP_ANS(ret);
  return ret;
}


/// 解ベクトルからBの盤面を計算する
void calcB(ans_t ans_cand){
  Fill(B, 0);
  for (size_t i = 0; i < ans_cand.size(); i++)
  {
    int y = ans_cand[i].pos.fi;
    int x = ans_cand[i].pos.se;
    int h = ans_cand[i].h;
    for (size_t j = max(0, y - h); j < min<int>(N, y + h); j++)
    {
      for (size_t k = max(0, x - h); k < min<int>(N, x + h); k++)
      {
        B[j][k] += max<int>(0, h - abs<int>(j - y) - abs<int>(k - x));
      }
    }
  }
  // debug("Finished calcB().\n");
  DUMP_B();
}


/// AとBの差分を計算する
ll calcDiff(){
  ll diff = 0;
  FOR(i,N)
    FOR(j,N)
      diff += abs(A[i][j] - B[i][j]);
  return diff;
}


/// 解ベクトルのスコアを計算する
ll calcScore(ans_t ans_cand){
  calcB(ans_cand);
  ll diff = calcDiff();
  debug("diff: %lld\n", diff);
  ll basePoints = BASE_POINT_MAX - diff;
  // TODO: calc bonus
  return basePoints;
}


/// 近傍解を生成する
void createBranch() {
  // debug("Starting createBranch()...\n");
  for (int i = 0; i < NUM_BRANCH; i++)
  {
    debug("Starting to alternate branch[%d]...\n", i);
   branch[i] = vector<Mountain>(Q);
    FOR(j,Q) branch[i][j] = ans[j]; // とりあえずansをコピー

    for(int k = 0; k < NOM; ++k)
    {
      // 1こ選んで
      int chg_index = randomDouble() * Q;
      // ガチランダムで適当に生成
      ll i_rand = randomDouble() * N;
      ll j_rand = randomDouble() * N;
      ll h_rand = randomDouble() * N + 1; // 1 =< H <= N

      // debug("\tbranch[%d][%d] = ans{pos{%d, %d}, h{%d}}\n",
      //   i, chg_index, i_rand, j_rand, h_rand);

      Mountain chg = Mountain{{i_rand, j_rand}, h_rand};
      branch[i][chg_index] = chg;
    }
    debug("Finished alternating branch[%d] to:\n\t", i);
    DUMP_ANS(branch[i]);
  }
  // debug("Finished createBranch().\n");
}


/// 近傍解と暫定解から次の暫定解を選択する
void selectBranch() {
  // debug("Starting selectBranch()...\n");
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
  if (ansScore != maxScore) {
    debug("Better solution found. Throwing a dice...\n");
    if (randomDouble() < threshold)
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
  // debug("Finished selectBranch().\n");
}


/// NOM(変化させる山型足し算の数。焼き鈍し法おける温度T)を低減する
void updateNOM() {
  NOM = Q * (double)(BASE_POINT_MAX - ansScore) / BASE_DIFF;
  DUMP(NOM);
}


/// 解の探索部メイン
void solve(){
  // Simulate Annealing
  // debug("Starting solve()...\n");

  for(annealing_time = 0; annealing_time < MAX_TIME && ansScore <= BASE_POINT_MAX; ++annealing_time) {
    debug("Starting search #%d...\n", annealing_time+1);
    createBranch();
    selectBranch();
    updateNOM();
  }

  // debug("Finished solve().\n");
}


/// 最終的な解の出力
void answer(){
  // output answer

  // Uncomment if double
  // cout << fixed << setprecision(8);
  cout << ans.size() << endl;
  FOR(i, ans.size()) printf("%d %d %lld\n",
    ans[i].pos.first,
    ans[i].pos.second,
    ans[i].h);
  debug("Finished answer().\nFinal Score is %lld.\n", ansScore);
}


/// 初期化処理
void init(){
  // 定数を表示
  debug("Given numbers:\n\tN: %d, Q: %d, MAX_TIME: %d, BASE_DIFF: %lld\n",
    N, Q, MAX_TIME, BASE_DIFF);

  // initialize for each test case
  initRandomFunction();

  Fill(B, 0); // Bを0埋め
  ans = generateRandomAnswer();
  ansScore = calcScore(ans);
  // debug("Finished init().\n");
}


/**
 * テスト用関数: ランダムなAとそれに対応する解ベクトルを生成し、保存する。
 * サンプルと異なるサイズで検証が可能。
 */
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
  debug ("Input board A loaded.\nA is:\n");
  DUMP_A();

  solve();

  #ifdef DEBUG
  debug();
  #endif

  answer();

  return 0;
}