#!/usr/bin/env perl

# usage:
# $ cat charm-list.txt | grep info | scripts/generate-input-script.pl > input.js
# で生成される input.js を, (devtool 等で) おまもり追加画面で実行する

use v5.12;
use warnings;
use diagnostics;

use utf8;
use Encode;

use JSON::PP qw/encode_json/;
use open IO => qw/:encoding(UTF-8) :std/;

my @input = map {
  /護石 \d (\d-\d-\d) (\D+)(\d)(?:,(\D+)(\d))?/
    ? {"第一スキル" => "$2", "第一スキルポイント" => "$3", "第二スキル" => "$4", "第二スキルポイント" => "$5", "スロット" => "$1"}
    : {}
} <>;

print <<EOF
const inputs = @{[decode('utf-8', encode_json \@input)]}

for (const input of inputs) {
  Object.entries(input).forEach(([key, value]) => {
    \$(`select[aria-label="\${key}"]`).val(value)
    \$(`select[aria-label="\${key}"]`)[0].dispatchEvent(new Event('change', { bubbles: true }))
  })

  \$('button:contains("追加")').click()

  break
}
EOF
