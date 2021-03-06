start_server {
    tags {"matrix"}
} {

    test {XSET} {
        assert_equal {2 2 2} [r xset xsetmat1 1 1 3]
        assert_equal {3 100 100 3} [r xset xsetmat2 99 99 2 255]
    }

    test {XGET} {
        assert_equal {2 4 4} [r xset xgetmat 3 3 1]
        assert_equal {2 4 4 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 1} [r xget xgetmat -1 -1]
    }

    test {XZEROS} {
      assert_equal {2 4 4} [r xzeros xzerosmat 4 4]
      assert_equal {2 4 4 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0} [r xget xzerosmat -1 -1]
    }

    test {XONES} {
      assert_equal {2 4 4} [r xones xonesmat 4 4]
      assert_equal {2 4 4 1 1 1 1 1 1 1 1 1 1 1 1 1 1 1 1} [r xget xonesmat -1 -1]
    }

    test {XEYE} {
      assert_equal {2 4 4} [r xeye identitymat 4 4]
      assert_equal {2 4 4 1 0 0 0 0 1 0 0 0 0 1 0 0 0 0 1} [r xget identitymat -1 -1]
    }
}
