# Lab 12 - Project Application

## E1 - Filter and ownership
Identify selected and excluded IDs, their reasons, and who owns each object.

* **Selected IDs:** 420, 180, 310, and 205. These orders met the criteria of being active `DELIVERY` orders with a status of either `PENDING` or `IN_PROGRESS`.

* **Excluded IDs:** 650 (excluded because is `DINE_IN`) and 900 (excluded because is `COMPLETED`).

* **Ownership:** The original `OrderPreview` objects are owned and stored statically in the base `orders` array within the `main` function. The `activeView` array functions that stores borrowed memory addresses (`&orders[index]`). Therefore, the view must never attempt to delete or deallocate these addresses.

## E2 - Report order and algorithm
List the observed report IDs and explain how each comparison rule affects them.
State the sorting algorithm used and its time and auxiliary-space bounds.

* **Observed Report Order:** Orders appear grouped by building in ascending alphabetical order, then sorted by priority in descending order, and finally ordered by `orderId` in ascending order for tie-breaking.

* **Comparison Rules:** 
  1. *Building:* Alphabetical comparison (`building` string comparison).
  2. *Priority:* Numeric comparison in descending order (higher priority values appear first).
  3. *Order ID:* Numeric comparison in ascending order to resolve any ties.

* **Sorting Algorithm and Complexity:** Quicksort was adapted to sort the pointer view. Its average time complexity is $O(n \log n)$ (with a worst-case of $O(n^2)$ for highly unbalanced partitions), and its auxiliary space complexity is $O(\log n)$ due to recursive call stack storage.

## E3 - Lookup choice and tests
* **Why Binary Search Cannot Be Used:** The report view is primarily sorted by building and priority, meaning that `orderId` values are not strictly in ascending order across the entire array. Consequently, binary search cannot be applied directly by ID.

* **Test Results:**
  * *Found test (ID 180):* Successfully found and retrieved with correct building, status, and priority details.
  * *Excluded test (ID 900):* Not present in the active view because its status was `COMPLETED`.
  * *Nonexistent test (ID 999):* Handled correctly, returning `-1` and indicating the order was not found.

* **Additional ID-Sorted View:** Maintaining a separate second view sorted exclusively by ID would consume additional auxiliary memory ($O(n)$ space for pointers) and require extra sorting overhead upon updates, but it would enable fast $O(\log n)$ binary searches by ID.

## E4 - Evidence and project connection
* **Source Code Reference:** `delivery_report_practice.cpp`
* **Output Evidence:** `delivery_report_output.txt`

* **Immutability Verification:** Verified by printing the original `orders` array before and after generating the report and executing lookups, confirming that all original field values and array positions remained completely intact.

* **Protection of Operational Sequence:** Separating the reporting view from the base collection protects the original FIFO operational sequence of the main queue, allowing administrative reporting and ID lookups without disrupting active café workflows.