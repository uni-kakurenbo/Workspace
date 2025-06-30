#! /bin/bash

# shellcheck source=/dev/null
function py-env() {
    source "${HOME}/python-env/bin/activate"
}

eval "$(
    oh-my-posh init --config ~/Main/terminal.omp.json bash
)"

function chrome() {
    "/mnt/c/Program Files/Google/Chrome/Application/chrome.exe" "$(wslpath -w "$(realpath "$1")")"
}

# shellcheck source=/dev/null
. "$HOME/Main/secrets.sh"

export USE_CCACHE=1
export CCACHE_DIR=/root/.ccache

export PATH="/home/uni_kakurenbo/.local/bin:$PATH"

export OPENSSL_INCLUDE_DIR="/usr/include/openssl"
export OPENSSL_DIR="/usr/include/openssl"
export OPENSSL_LIB_DIR="/usr/lib/x86_64-linux-gnu"

alias colors="~/.local/lib/256-colors.sh"

alias ccore="~/Main/CompetitiveProgramming/commands/ccore.sh"
alias slc="source ~/Main/CompetitiveProgramming/commands/focus.sh"
alias abc="slc abc"
alias arc="slc arc"
alias agc="slc agc"
alias ahc="slc ahc"
alias op="~/Main/CompetitiveProgramming/commands/open.sh"
alias fd="~/Main/CompetitiveProgramming/commands/find.sh"
alias jp="~/Main/CompetitiveProgramming/commands/jump.sh"
alias ts="~/Main/CompetitiveProgramming/commands/test.sh"
alias sub="~/Main/CompetitiveProgramming/commands/submit.sh"
alias clp="~/Main/CompetitiveProgramming/commands/clip.sh"
alias docs="~/Main/CompetitiveProgramming/commands/docs.sh"
alias pre-compile="~/Main/CompetitiveProgramming/commands/pre-compile.sh"

alias open="powershell.exe /c start"

alias remote="git remote get-url origin | open"
alias rebase="git rebase -i --autosquash"
alias fixup="git commit --fixup"
alias main="git switch main"
alias push="git status"
alias push="git push"
alias prune-test="git branch --list 'test/*' | xargs git branch -D"

function delete() {
    git branch -D "$1"
    git branch -D "test/$1"
}

alias drop-cache="sudo bash -c 'echo 3 >/proc/sys/vm/drop_caches && swapoff -a && swapon -a'"

function remove-submodule() {
    git submodule deinit -f "$1"
    git rm -f "$1"
    rm -rf ".git/modules/$1"
}

# shellcheck source=/dev/null
. "$HOME/.cargo/env"

export NVM_DIR="$HOME/.nvm"
[ -s "$NVM_DIR/nvm.sh" ] && \. "$NVM_DIR/nvm.sh"                   # This loads nvm
[ -s "$NVM_DIR/bash_completion" ] && \. "$NVM_DIR/bash_completion" # This loads nvm bash_completion

py-env
