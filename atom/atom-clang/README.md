### Note: I no longer maintain this. I found Visual Studio Code + https://marketplace.visualstudio.com/items?itemName=ms-vscode.cpptools is better at this point. It is not perfect, but more features and I find Code to be faster than Atom.

# atom-clang

[LibClang](http://clang.llvm.org/docs/Tooling.html) based code completion and linter for [Atom Editor](http://atom.io)

![](https://cloud.githubusercontent.com/assets/156174/17979392/8c78bf54-6ab7-11e6-97bf-f7de4eedc56b.gif)

## Features

* Semantic code completion using [autocomplete-plus](https://atom.io/packages/autocomplete-plus)
* Diagnostics using [linter](https://atom.io/packages/linter)
* Native C++ v8 Node module using [LibClang](http://clang.llvm.org/docs/Tooling.html), no 3rd party dependencies.
  * Fast low-latency completions
  * Easy to setup with your project, no external configuration needed
* Per project configuration supported with [Project Manager](https://atom.io/packages/project-manager)

## Requirements

Since this is a package uses C++, it will require the package to be compiled. This is automatically done for you by Atom, but it does have some requirements.

* C++11 compiler, either GCC or Clang
* LibClang 3.8 or greater

LibClang is detected via `llvm-config` command. It needs to build and link against the LibClang headers and library. It attempts to find the newest version of `llvm-config` it can on your system. One can override the search by having the `LLVM_CONFIG` environment variable set, e.g. `LLVM_CONFIG=/opt/clang+llvm-3.9.0-x86_64-linux-gnu-ubuntu-16.04/bin/llvm-config`. Currently, I do not know of way "at package install time" through Atom to let the user specify which one to use. See [llvm-config.sh](https://github.com/joeroback/atom-clang/blob/master/llvm-config.sh).

On Linux, any distro provided libclang versions 3.8 or 3.8 should work, [LLVM's Apt](http://apt.llvm.org/), or directly from [LLVM](http://llvm.org). On Mac, the easiest way to upgrade to an atom-clang compatible LLVM version is with [MacPorts](http://macports.org/), [Homebrew](http://brew.sh/), or directly from [LLVM](http://llvm.org/).

Currently, this only works on Linux and Mac. I do not have a Windows computer. I would welcome any patches / pull requests to add Windows support.

### Installing

This package should be installed through the Atom package manager. Search for package named [atom-clang](https://atom.io/packages/atom-clang). For the bleeding edge, you can clone this repository and link it into Atom with `apm` if desired.

### Global Configuration

This package can work out-of-the-box, but usually requires some minor configuration. This is because parsing with LibClang, literally runs `clang` over your source files and usually needs the `CFLAGS` or `CXXFLAGS` one would use to compile.

You can set global `CFLAGS` and `CXXFLAGS`. This will work for any random projects without any major flags.

### Per Project Configuration

Per project configuration with Atom is an ongoing open feature request. Thankfully, [Project Manager](https://atom.io/packages/project-manager) gives us at least 99% of what we want. It does require you to manually edit the project settings file.

![](https://cloud.githubusercontent.com/assets/156174/18150928/f3d07f4e-6fa7-11e6-957c-3068969061f2.png)

```CoffeeScript
my_project:
  title: "Test Project"
  paths: [
    "/home/user/my_project"
  ]
  icon: "icon-file-code"
  settings:
    "*":
      "atom-clang":
        linterEnabled: true
        includeDeprecated: false
        defaultCFlags: [
          "-std=c99"
          "-Wall"
          "-Wextra"
          "-D_REENTRANT"
          "-D_GNU_SOURCE"
          "-I /opt/local/include"
          "-I /home/user/my_project"
        ]
        defaultCXXFlags: [
          "-std=c++1z"
          "-Wall"
          "-Wextra"
          "-D_REENTRANT"
          "-D_GNU_SOURCE"
          "-fexceptions"
          "-I /opt/local/include"
          "-I /home/user/my_project"
        ]
```

## Keymaps

* TODO: <kbd>ctrl</kbd>+<kbd>alt</kbd>+<kbd>r</kbd> Reparse Current Editor
* TODO: <kbd>shift</kbd>+<kbd>ctrl</kbd>+<kbd>alt</kbd>+<kbd>r</kbd> Reparse All Editors

## Issues

Please report all issues in the [issue tracker](https://github.com/joeroback/atom-clang/issues).

The more information you can include, the better. CFlags/CXXFlags especially.
Having a reproducible test case would also greatly increase the debug time, though I realize that is not always possible.

## Roadmap

I would love the roadmap to be largely driven by users, but her is a list I came up to get started.

#### Short Term

* Add status bar indications for background processing of parsing, completions... Some large C++ can take time to parse/complete..
* Add support for compile_commands.json
* Add support for precompiled headers

#### Long Term

* Replace [linter](https://atom.io/packages/linter) (I find the interface unsuited for C++ linting, and to be frank, very buggy)
  * Add support for FixIt
* Add support for <kbd>ctrl</kbd>-click code navigation

## License

MIT License

Copyright (c) 2016, Joe Roback &lt;<joe.roback@gmail.com>&gt;

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in all
copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
SOFTWARE.
