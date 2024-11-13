# DoXOR

This creates a file content checking by xor-ing its content.

**Do not trust in this project to generate secure file content.**

## Usage

DoXOR *file_name*

This generates a XOR data from the file content

*Sample result:*
**642021285665477A23233D286834646D426C6A2A2D100937406C65732A2F0A2E2F436172676F2E6C6F636B0A2E2F434D616B6543616368652E7478740A2E2F63**

DoXOR *file_name* *check_data*

This gets the *file_name* content, creates its XOR data and compares with the *check_data*.

**NOTE:** The XOR checking data size **must** be 128 bytes.

## Languages

### C++

File: <code>./src/DoXOR.cpp</code>.

Build: <code>cmake .</code> and then <code>make</code>.

Test: <code>./DoXOR *file_name* *[checking data]*</code> or see usage...

### Rust

Files: <code>./src/main.rs</code> and <code>Cargo.toml</code>.

Build: <code>cargo build --release</code>.

Test: <code>cargo run *file_name* *[checking data]*</code> or see usage...

### Java

Files: <code>./src/DoXOR.java</code> and <code>manifest.txt</code>.

Build: <code><TBD></code>.

Test: <code><TBD> *file_name* *[checking data]*</code> or see usage...

### Python Scripts

The <code>./src/DoXOR.py</code> has the same functionality provided by the C++/Rust application (*DoXOR file_name*).

The <code>./src/DoHMAC_MD5.py</code> was just a test using MD5 hash.

Both are supposed to be Python 2.7 and Python 3.4 compatible.

They were not tested in any other version than those.

### Full build and test

Run everything at one using the <code>build.sh</code> (tested only on MacOS High Sierra).

This simple script will create the 'deliverables' and run simple tests.

## License

See LICENSE file.