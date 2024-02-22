.empty());
}

TEST(Btree, SwissTableHashable) {
  static constexpr int kValues = 10000;
  std::vector<int> values(kValues);
  std::iota(values.begin(), values.end(), 0);
  std::vector<std::pair<int, int>> map_values;
  for (int v : values) map_values.emplace_back(v, -v);

  using set = absl::btree_set<int>;
  EXPECT_TRUE(absl::VerifyTypeImplementsAbslHashCorrectly({
      set{},
      set{1},
      set{2},
      set{1, 2},
      set{2, 1},
      set(values.begin(), values.end()),
      set(values.rbegin(), values.rend()),
  }));

  using mset = absl::btree_multiset<int>;
  EXPECT_TRUE(absl::VerifyTypeImplementsAbslHashCorrectly({
      mset{},
      mset{1},
      mset{1, 1},
      mset{2},
      mset{2, 2},
      mset{1, 2},
      mset{1, 1, 2},
      mset{1, 2, 2},
      mset{1, 1, 2, 2},
      mset(values.begin(), values.end()),
      mset(values.rbegin(), values.rend()),
  }));

  using map = absl::btree_map<int, int>;
  EXPECT_TRUE(absl::VerifyTypeImplementsAbslHashCorrectly({
      map{},
      map{{1, 0}},
      map{{1, 1}},
      map{{2, 0}},
      map{{2, 2}},
      map{{1, 0}, {2, 1}},
      map(map_values.begin(), map_values.end()),
      map(map_values.rbegin(), map_values.rend()),
  }));

  using mmap = absl::btree_multimap<int, int>;
  EXPECT_TRUE(absl::VerifyTypeImplementsAbslHashCorrectly({
      mmap{},
      mmap{{1, 0}},
      mmap{{1, 1}},
      mmap{{1, 0}, {1, 1}},
      mmap{{1, 1}, {1, 0}},
      mmap{{2, 0}},
      mmap{{2, 2}},
      mmap{{1, 0}, {2, 1}},
      mmap(map_values.begin(), map_values.end()),
      mmap(map_values.rbegin(), map_values.rend()),
  }));
}

TEST(Btree, ComparableSet) {
  absl::btree_set<int> s1 = {1, 2};
  absl::btree_set<int> s2 = {2, 3};
  EXPECT_LT(s1, s2);
  EXPECT_LE(s1, s2);
  EXPECT_LE(s1, s1);
  EXPECT_GT(s2, s1);
  EXPECT_GE(s2, s1);
  EXPECT_GE(s1, s1);
}

TEST(Btree, ComparableSetsDifferentLength) {
  absl::btree_set<int> s1 = {1, 2};
  absl::btree_set<int> s2 = {1, 2, 3};
  EXPECT_LT(s1, s2);
  EXPECT_LE(s1, s2);
  EXPECT_GT(s2, s1);
  EXPECT_GE(s2, s1);
}

TEST(Btree, ComparableMultiset) {
  absl::btree_multiset<int> s1 = {1, 2};
  absl::btree_multiset<int> s2 = {2, 3};
  EXPECT_LT(s1, s2);
  EXPECT_LE(s1, s2);
  EXPECT_LE(s1, s1);
  EXPECT_GT(s2, s1);
  EXPECT_GE(s2, s1);
  EXPECT_GE(s1, s1);
}

TEST(Btree, ComparableMap) {
  absl::btree_map<int, int> s1 = {{1, 2}};
  absl::btree_map<int, int> s2 = {{2, 3}};
  EXPECT_LT(s1, s2);
  EXPECT_LE(s1, s2);
  EXPECT_LE(s1, s1);
  EXPECT_GT(s2, s1);
  EXPECT_GE(s2, s1);
  EXPECT_GE(s1, s1);
}

TEST(Btree, ComparableMultimap) {
  absl::btree_multimap<int, int> s1 = {{1, 2}};
  absl::btree_multimap<int, int> s2 = {{2, 3}};
  EXPECT_LT(s1, s2);
  EXPECT_LE(s1, s2);
  EXPECT_LE(s1, s1);
  EXPECT_GT(s2, s1);
  EXPECT_GE(s2, s1);
  EXPECT_GE(s1, s1);
}

TEST(Btree, ComparableSetWithCustomComparator) {
  // As specified by
  // http://www.open-std.org/jtc1/sc22/wg21/docs/papers/2012/n3337.pdf section
  // [container.requirements.general].12, ordering associative containers always
  // uses default '<' operator
  // - even if otherwise the container uses custom functor.
  absl::btree_set<int, std::greater<int>> s1 = {1, 2};
  absl::btree_set<int, std::greater<int>> s2 = {2, 3};
  EXPECT_LT(s1, s2);
  EXPECT_LE(s1, s2);
  EXPECT_LE(s1, s1);
  EXPECT_GT(s2, s1);
  EXPECT_GE(s2, s1);
  EXPECT_GE(s1, s1);
}

TEST(Btree, EraseReturnsIterator) {
  absl::btree_set<int> set = {1, 2, 3, 4, 5};
  auto result_it = set.erase(set.begin(), set.find(3));
  EXPECT_EQ(result_it, set.find(3));
  result_it = set.erase(set.find(5));
  EXPECT_EQ(result_it, set.end());
}

TEST(Btree, ExtractAndInsertNodeHandleSet) {
  absl::btree_set<int> src1 = {1, 2, 3, 4, 5};
  auto nh = src1.extract(src1.find(3));
  EXPECT_THAT(src1, ElementsAre(1, 2, 4, 5));
  absl::btree_set<int> other;
  absl::btree_set<int>::insert_return_type res = other.insert(std::move(nh));
  EXPECT_THAT(other, ElementsAre(3));
  EXPECT_EQ(res.position, other.find(3));
  EXPECT_TRUE(res.inserted);
  EXPECT_TRUE(res.node.empty());

  absl::btree_set<int> src2 = {3, 4};
  nh = src2.extract(src2.find(3));
  EXPECT_THAT(src2, ElementsAre(4));
  res = other.insert(std::move(nh));
  EXPECT_THAT(other, ElementsAre(3));
  EXPECT_EQ(res.position, other.find(3));
  EXPECT_FALSE(res.inserted);
  ASSERT_FALSE(res.node.empty());
  EXPECT_EQ(res.node.value(), 3);
}

template <typename Set>
void TestExtractWithTrackingForSet() {
  InstanceTracker tracker;
  {
    Set s;
    // Add enough elements to make sure we test internal nodes too.
    const size_t kSize = 1000;
    while (s.size() < kSize) {
      s.insert(MovableOnlyInstance(s.size()));
    }
    for (int i = 0; i < kSize; ++i) {
      // Extract with key
      auto nh = s.extract(MovableOnlyInstance(i));
      EXPECT_EQ(s.size(), kSize - 1);
      EXPECT_EQ(nh.value().value(), i);
      // Insert with node
      s.insert(std::move(nh));
      EXPECT_EQ(s.size(), kSize);

      // Extract with iterator
      auto it = s.find(MovableOnlyInstance(i));
      nh = s.extract(it);
      EXPECT_EQ(s.size(), kSize - 1);
      EXPECT_EQ(nh.value().value(), i);
      // Insert with node and hint
      s.insert(s.begin(), std::move(nh));
      EXPECT_EQ(s.size(), kSize);
    }
  }
  EXPECT_EQ(0, tracker.instances());
}

template <typename Map>
void TestExtractWithTrackingForMap() {
  InstanceTracker tracker;
  {
    Map m;
    // Add enough elements to make sure we test internal nodes too.
    const size_t kSize = 1000;
    while (m.size() < kSize) {
      m.insert(
          {CopyableMovableInstance(m.size()), MovableOnlyInstance(m.size())});
    }
    for (int i = 0; i < kSize; ++i) {
      // Extract with key
      auto nh = m.extract(CopyableMovableInstance(i));
      EXPECT_EQ(m.size(), kSize - 1);
      EXPECT_EQ(nh.key().value(), i);
      EXPECT_EQ(nh.mapped().value(), i);
      // Insert with node
      m.insert(std::move(nh));
      EXPECT_EQ(m.size(), kSize);

      // Extract with iterator
      auto it = m.find(CopyableMovableInstance(i));
      nh = m.extract(it);
      EXPECT_EQ(m.size(), kSize - 1);
      EXPECT_EQ(nh.key().value(), i);
      EXPECT_EQ(nh.mapped().value(), i);
      // Insert with node and hint
      m.insert(m.begin(), std::move(nh));
      EXPECT_EQ(m.size(), kSize);
    }
  }
  EXPECT_EQ(0, tracker.instances());
}

TEST(Btree, ExtractTracking) {
  TestExtractWithTrackingForSet<absl::btree_set<MovableOnlyInstance>>();
  TestExtractWithTrackingForSet<absl::btree_multiset<MovableOnlyInstance>>();
  TestExtractWithTrackingForMap<
      absl::btree_map<CopyableMovableInstance, MovableOnlyInstance>>();
  TestExtractWithTrackingForMap<
      absl::btree_multimap<CopyableMovableInstance, MovableOnlyInstance>>();
}

TEST(Btree, ExtractAndInsertNodeHandleMultiSet) {
  absl::btree_multiset<int> src1 = {1, 2, 3, 3, 4, 5};
  auto nh = src1.extract(src1.find(3));
  EXPECT_THAT(src1, ElementsAre(1, 2, 3, 4, 5));
  absl::btree_multiset<int> other;
  auto res = other.insert(std::move(nh));
  EXPECT_THAT(other, ElementsAre(3));
  EXPECT_EQ(res, other.find(3));

  absl::btree_multiset<int> src2 = {3, 4};
  nh = src2.extract(src2.find(3));
  EXPECT_THAT(src2, ElementsAre(4));
  res = other.insert(std::move(nh));
  EXPECT_THAT(other, ElementsAre(3, 3));
  EXPECT_EQ(res, ++other.find(3));
}

TEST(Btree, ExtractAndInsertNodeHandleMap) {
  absl::btree_map<int, int> src1 = {{1, 2}, {3, 4}, {5, 6}};
  auto nh = src1.extract(src1.find(3));
  EXPECT_THAT(src1, ElementsAre(Pair(1, 2), Pair(5, 6)));
  absl::btree_map<int, int> other;
  absl::btree_map<int, int>::insert_return_type res =
      other.insert(std::move(nh));
  EXPECT_THAT(other, ElementsAre(Pair(3, 4)));
  EXPECT_EQ(res.position, other.find(3));
  EXPECT_TRUE(res.inserted);
  EXPECT_TRUE(res.node.empty());

  absl::btree_map<int, int> src2 = {{3, 6}};
  nh = src2.extract(src2.find(3));
  EXPECT_TRUE(src2.empty());
  res = other.insert(std::move(nh));
  EXPECT_THAT(other, ElementsAre(Pair(3, 4)));
  EXPECT_EQ(res.position, other.find(3));
  EXPECT_FALSE(res.inserted);
  ASSERT_FALSE(res.node.empty());
  EXPECT_EQ(res.node.key(), 3);
  EXPECT_EQ(res.node.mapped(), 6);
}

TEST(Btree, ExtractAndInsertNodeHandleMultiMap) {
  absl::btree_multimap<int, int> src1 = {{1, 2}, {3, 4}, {5, 6}};
  auto nh = src1.extract(src1.find(3));
  EXPECT_THAT(src1, ElementsAre(Pair(1, 2), Pair(5, 6)));
  absl::btree_multimap<int, int> other;
  auto res = other.insert(std::move(nh));
  EXPECT_THAT(other, ElementsAre(Pair(3, 4)));
  EXPECT_EQ(res, other.find(3));

  absl::btree_multimap<int, int> src2 = {{3, 6}};
  nh = src2.extract(src2.find(3));
  EXPECT_TRUE(src2.empty());
  res = other.insert(std::move(nh));
  EXPECT_THAT(other, ElementsAre(Pair(3, 4), Pair(3, 6)));
  EXPECT_EQ(res, ++other.begin());
}

TEST(Btree, ExtractMultiMapEquivalentKeys) {
  // Note: using string keys means a three-way comparator.
  absl::btree_multimap<std::string, int> map;
  for (int i = 0; i < 100; ++i) {
    for (int j = 0; j < 100; ++j) {
      map.insert({absl::StrCat(i), j});
    }
  }

  for (int i = 0; i < 100; ++i) {
    const std::string key = absl::StrCat(i);
    auto node_handle = map.extract(key);
    EXPECT_EQ(node_handle.key(), key);
    EXPECT_EQ(node_handle.mapped(), 0) << i;
  }

  for (int i = 0; i < 100; ++i) {
    const std::string key = absl::StrCat(i);
    auto node_handle = map.extract(key);
    EXPECT_EQ(node_handle.key(), key);
    EXPECT_EQ(node_handle.mapped(), 1) << i;
  }
}

TEST(Btree, ExtractAndGetNextSet) {
  absl::btree_set<int> src = {1, 2, 3, 4, 5};
  auto it = src.find(3);
  auto extracted_and_next = src.extract_and_get_next(it);
  EXPECT_THAT(src, ElementsAre(1, 2, 4, 5));
  EXPECT_EQ(extracted_and_next.node.value(), 3);
  EXPECT_EQ(*extracted_and_next.next, 4);
}

TEST(Btree, ExtractAndGetNextMultiSet) {
  absl::btree_multiset<int> src = {1, 2, 3, 4, 5};
  auto it = src.find(3);
  auto extracted_and_next = src.extract_and_get_next(it);
  EXPECT_THAT(src, ElementsAre(1, 2, 4, 5));
  EXPECT_EQ(extracted_and_next.node.value(), 3);
  EXPECT_EQ(*extracted_and_next.next, 4);
}

TEST(Btree, ExtractAndGetNextMap) {
  absl::btree_map<int, int> src = {{1, 2}, {3, 4}, {5, 6}};
  auto it = src.find(3);
  auto extracted_and_next = src.extract_and_get_next(it);
  EXPECT_THAT(src, ElementsAre(Pair(1, 2), Pair(5, 6)));
  EXPECT_EQ(extracted_and_next.node.key(), 3);
  EXPECT_EQ(extracted_and_next.node.mapped(), 4);
  EXPECT_THAT(*extracted_and_next.next, Pair(5, 6));
}

TEST(Btree, ExtractAndGetNextMultiMap) {
  absl::btree_multimap<int, int> src = {{1, 2}, {3, 4}, {5, 6}};
  auto it = src.find(3);
  auto extracted_and_next = src.extract_and_get_next(it);
  EXPECT_THAT(src, ElementsAre(Pair(1, 2), Pair(5, 6)));
  EXPECT_EQ(extracted_and_next.node.key(), 3);
  EXPECT_EQ(extracted_and_next.node.mapped(), 4);
  EXPECT_THAT(*extracted_and_next.next, Pair(5, 6));
}

TEST(Btree, ExtractAndGetNextEndIter) {
  absl::btree_set<int> src = {1, 2, 3, 4, 5};
  auto it = src.find(5);
  auto extracted_and_next = src.extract_and_get_next(it);
  EXPECT_THAT(src, ElementsAre(1, 2, 3, 4));
  EXPECT_EQ(extracted_and_next.node.value(), 5);
  EXPECT_EQ(extracted_and_next.next, src.end());
}

TEST(Btree, ExtractDoesntCauseExtraMoves) {
#ifdef _MSC_VER
  GTEST_SKIP() << "This test fails on MSVC.";
#endif

  using Set = absl::btree_set<MovableOnlyInstance>;
  std::array<std::function<void(Set &)>, 3> extracters = {
      [](Set &s) { auto node = s.extract(s.begin()); },
      [](Set &s) { auto ret = s.extract_and_get_next(s.begin()); },
      [](Set &s) { auto node = s.extract(MovableOnlyInstance(0)); }};

  InstanceTracker tracker;
  for (int i = 0; i < 3; ++i) {
    Set s;
    s.insert(MovableOnlyInstance(0));
    tracker.ResetCopiesMovesSwaps();

    extracters[i](s);
    // We expect to see exactly 1 move: from the original slot into the
    // extracted node.
    EXPECT_EQ(tracker.copies(), 0) << i;
    EXPECT_EQ(tracker.moves(), 1) << i;
    EXPECT_EQ(tracker.swaps(), 0) << i;
  }
}

// For multisets, insert with hint also affects correctness because we need to
// insert immediately before the hint if possible.
struct InsertMultiHintData {
  int key;
  int not_key;
  bool operator==(const InsertMultiHintData other) const {
    return key == other.key && not_key == other.not_key;
  }
};

struct InsertMultiHintDataKeyCompare {
  using is_transparent = void;
  bool operator()(const InsertMultiHintData a,
                  const InsertMultiHintData b) const {
    return a.key < b.key;
  }
  bool operator()(const int a, const InsertMultiHintData b) const {
    return a < b.key;
  }
  bool operator()(const InsertMultiHintData a, const int b) const {
    return a.key < b;
  }
};

TEST(Btree, InsertHintNodeHandle) {
  // For unique sets, insert with hint is just a performance optimization.
  // Test that insert works correctly when the hint is right or wrong.
  {
    absl::btree_set<int> src = {1, 2, 3, 4, 5};
    auto nh = src.extract(src.find(3));
    EXPECT_THAT(src, ElementsAre(1, 2, 4, 5));
    absl::btree_set<int> other = {0, 100};
    // Test a correct hint.
    auto it = other.insert(other.lower_bound(3), std::move(nh));
    EXPECT_THAT(other, ElementsAre(0, 3, 100));
    EXPECT_EQ(it, other.find(3));

    nh = src.extract(src.find(5));
    // Test an incorrect hint.
    it = other.insert(other.end(), std::move(nh));
    EXPECT_THAT(other, ElementsAre(0, 3, 5, 100));
    EXPECT_EQ(it, other.find(5));
  }

  absl::btree_multiset<InsertMultiHintData, InsertMultiHintDataKeyCompare> src =
      {{1, 2}, {3, 4}, {3, 5}};
  auto nh = src.extract(src.lower_bound(3));
  EXPECT_EQ(nh.value(), (InsertMultiHintData{3, 4}));
  absl::btree_multiset<InsertMultiHintData, InsertMultiHintDataKeyCompare>
      other = {{3, 1},