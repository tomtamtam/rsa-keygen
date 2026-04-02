#include <iostream>
#include <ctime>
#include <cstdlib>
#include <cmath>
#include <tuple>
#include <optional>
#include <cstdint>
#include <string>
#include <filesystem>
#include <fstream>
#include <charconv>

int64_t to_int64(const std::string& s) {
    int64_t val = 0;
    auto [ptr, ec] = std::from_chars(s.data(), s.data() + s.size(), val);
    if (ec == std::errc() && ptr == s.data() + s.size()) return val;
  return -1;
}

static uint64_t modmul(uint64_t a, uint64_t b, uint64_t m) {
    return (uint64_t)((__int128)a * b % m);
}

static uint64_t modpow(uint64_t base, uint64_t exp, uint64_t mod) {
    base %= mod;
    uint64_t res = 1;
    while (exp) {
        if (exp & 1) res = modmul(res, base, mod);
        base = modmul(base, base, mod);
        exp >>= 1;
    }
    return res;
}

static std::tuple<int64_t,int64_t,int64_t> egcd(int64_t a, int64_t b) {
    if (b == 0) return {a, 1, 0};
    auto [g, x1, y1] = egcd(b, a % b);
    return {g, y1, x1 - (a / b) * y1};
}

static std::optional<int64_t> modinv(int64_t a, int64_t m) {
    a %= m;
    if (a < 0) a += m;
    auto [g, x, y] = egcd(a, m);
    if (g != 1) return std::nullopt; // inverse doesn't exist
    int64_t inv = x % m;
    if (inv < 0) inv += m;
    return inv;
}

static int64_t randRange(int64_t lo, int64_t hi)
{
  return lo + std::rand() % (hi - lo + 1);
}

static bool fermatTest(int64_t n, int64_t k)
{
  if(n == 2)
  {
    return true;
  }
  if(n % 2 == 0)
  {
    return false;
  }
  for(int64_t i = 0; i < k; i ++)
  {
    int a = randRange(1, n - 1);
    if(modpow(a, n - 1, n))
    {
      return false;
    }
    return true;
  }
}

static int getPrime(int64_t lower, int64_t higher, int64_t fermat_loops)
{
  while(true)
  {
    int64_t r = randRange(lower, higher);
    if(fermatTest(r, fermat_loops))
    {
      return r;
    }
  }
}

static std::pair<std::pair<int64_t, int64_t>, std::pair<int64_t, int64_t>> gen_keys(int64_t min, int64_t max, int64_t fermat_loops)
{
  int64_t p = getPrime(min, max, fermat_loops);
  int64_t q = getPrime(min, max, fermat_loops);

  int64_t N = p * q;
  int64_t c = (p - 1) * (q - 1);

  int64_t e = 65537;
  auto inv = modinv(e, c);

  int64_t d = 0;
  if(inv)
  {
    d = *inv;
  }

  std::pair<int64_t, int64_t> public_key = {e, N};
  std::pair<int64_t, int64_t> private_key = {d, N};

  return {public_key, private_key};
}

int main(int argc, char* argv[])
{
  std::srand(static_cast<unsigned>(std::time(nullptr)));

  std::string path = std::filesystem::current_path().string() + "/key";
  int64_t min = 1000000000;
  int64_t max = 9223372036854775807;
  int64_t fermat_repeat = 10;

  bool is_param = false;
  std::string param = "";

  for(int i = 1; i < argc; i++)
  {
    std::string argument = argv[i];

    if(argument.starts_with("-"))
    {
      is_param = true;
     
      param = argument.substr(1);
      if(param == "h" || param == "help")
      {
        std::cout << "use as followed:\nrsa-keygen                            | creates 2 files in current dir (key, key.pub)\nrsa-keygen ~/home/user/folder/testkey | creates 2 files in ~/home/user/folder/testkey (key, key.pub)\nrsa-keygen -param value               | sets the parameter [min, max, fermat_repeat] to the given value\n";
        return 0;
      }
    }
    else 
    {
      if(is_param)
      {
        is_param = false;
        if(param == "min")
        {
          min = to_int64(argument);
          std::cout << "min arg, min: " << min << '\n';
        }
        else if(param == "max")
        {
          max = to_int64(argument);
          std::cout << "max arg, max: " << max << '\n';
        }
        else if(param == "fermat_repeat")
        {
          fermat_repeat = to_int64(argument);
          std::cout << "repeat arg, rep: " << fermat_repeat << '\n';
        }
      else
      {
        path = argument;
      }
    }
  
    }}

  auto keys = gen_keys(min, max, fermat_repeat);
  std::cout <<"test";
  std::cout << "pubk:\n" << keys.first.first << '\n' << keys.first.second << "\n\nprivK:\n" << keys.second.first << '\n' << keys.second.second; 

  std::ofstream kPriv(path, std::ios::trunc);
  if(kPriv)
  {
    std::cout << "priv key at " << path << '\n';
    kPriv << keys.second.first << '\n' << keys.second.second;
    kPriv.close();
  }

  std::string public_path = path + ".pub";

  std::ofstream kPub(public_path, std::ios::trunc);
  if(kPub)
  {
    std::cout << "pub key at " << public_path << '\n';
    kPub << keys.first.first << '\n' << keys.first.second;
    kPub.close();
  }
}
