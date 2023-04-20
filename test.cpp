
// Project file headers
#include "Script.hpp"
#include "PNG.hpp"
// C++ library headers
#include <algorithm>
#include <cstdlib>
#include <cassert>
#include <iostream>
#include <iomanip>
#include <string>
#include <vector>
#include <iterator>
#include <fstream>
using namespace std;

// POSIX headers
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <dirent.h>

namespace prog
{

  const string LOG_FILE_NAME = "test_log.txt";

  class TestDriver
  {
  private:
    string root_path;
    int total_tests = 0;
    int passed_tests = 0;
    int failed_tests = 0;
    FILE *log_stream;

    bool test_script(const string &id)
    {
      string script_file = root_path + "/scripts/" + id + ".txt";
      string out_file = root_path + "/output/" + id + ".png";
      string exp_file = root_path + "/expected/" + id + ".png";
      Script s(script_file);
      s.run();
      return comparePNG(exp_file, out_file, cout);
    }

    void onTestBegin(const string &id)
    {
      total_tests++;
      fprintf(log_stream, ">>>> [%d] %s <<<<\n", total_tests, id.c_str());
      fflush(log_stream);
      cout << '[' << total_tests << "] " << id << ": ";
      cout.flush();
    }
    void onTestCompletion(bool success)
    {
      cout << (success ? "pass" : "fail") << std::endl;
      if (success)
      {
        passed_tests++;
      }
      else
      {
        failed_tests++;
      }
    }

    
    void run_test(const string &script_id)
    {
      int log_fd = ::fileno(log_stream);
      onTestBegin(script_id);
      ::pid_t pid = ::fork();

      if (pid == 0)
      {
        ::dup2(log_fd, 1);
        ::dup2(log_fd, 2);
        bool success = test_script(script_id);
        ::exit(success ? 0 : 1);
      }
      else if (pid > 0)
      {
        // parent process waits
        int child_status = -1;
        ::waitpid(pid, &child_status, 0);

        bool success = WIFEXITED(child_status) &&
                       WEXITSTATUS(child_status) == 0;
        onTestCompletion(success);
      }
      else
      {
        // pid < 0
        perror("Unable to run tests! Process creation failed!");
        exit(1);
      }
    }

  public:
    TestDriver(const string &root_path)
        : root_path(root_path),
          log_stream(fopen((root_path + "/" + LOG_FILE_NAME).c_str(), "w"))
    {

    }
    void color_tests() {
      const Color a(1, 2, 3);
      assert(a.red() == 1);
      assert(a.green() == 2);
      assert(a.blue() == 3);

      const Color b;
      assert(b.red() == 0);
      assert(b.green() == 0);
      assert(b.blue() == 0);
 
      const Color c(b);
      assert(c.red() == b.red());
      assert(c.green() == b.green());
      assert(c.blue() == b.blue());

      Color d(4, 5, 6);
      d.red() = 111;
      d.green() = 255;
      d.blue() = 33;
      assert(d.red() == 111);
      assert(d.green() == 255);
      assert(d.blue() == 33);
      cout << "Color tests passed!" << endl;
    }

    void test_scripts(const string &spec)
    {
      DIR *directory = ::opendir((root_path + "/scripts").c_str());
      if (directory == nullptr)
      {
        cerr << "Unable to open scripts directory!" << endl;
        return;
      }
      vector<string> scripts_to_execute;
      ::dirent *entry;
      while ((entry = readdir(directory)) != nullptr)
      {
        if (entry->d_type == DT_REG)
        {
          string fname = entry->d_name;
          if (fname.find(spec) != string::npos)
          {
            scripts_to_execute.push_back(fname.substr(0, fname.find_last_of('.')));
          }
        }
      }
      ::closedir(directory);
      if (scripts_to_execute.empty())
      {
        cout << "No scripts matched the spec: " << spec << endl;
        return;
      }
      sort(scripts_to_execute.begin(), scripts_to_execute.end());

      cout << "== " << scripts_to_execute.size() << " tests to execute  ==" << endl;
      for (string id : scripts_to_execute)
      {
        run_test(id);
      }

      cout << "== TEST EXECUTION SUMMARY ==" << endl
           << "Total tests: " << total_tests << endl
           << "Passed tests: " << passed_tests << endl
           << "Failed tests: " << failed_tests << endl
           << "See " << LOG_FILE_NAME << " for details." << endl;
    }
  };
}

int main(int argc, char **argv)
{
  --argc;
  ++argv;
  prog::TestDriver driver(argc == 2 ? argv[1] : ".");
  string spec = argc >=1 ? argv[0] : "";
  if (spec == "Color") {
    driver.color_tests();
  } else {
    driver.test_scripts(spec);
  }
  return 0;
}
