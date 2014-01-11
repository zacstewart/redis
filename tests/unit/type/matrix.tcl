start_server {
    tags {"matrix"}
} {

    test {XSET} {
        assert_equal {2 2} [r xset mymatrix 1 1 3]
    }
}
