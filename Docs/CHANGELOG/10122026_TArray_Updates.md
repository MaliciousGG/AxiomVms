# CHANGELOG October - 12, 2026
## TArray.h/inl
### Memory Management
- `Fixed:` Replaced mixed `new[]`, `delete[]`, and raw allocation with matching `std::allocator<T>::allocate()` and `deallocate()` calls.
- `Fixed:` Copy construction now allocates raw storage and individually constructs copied elements.
- `Fixed:` Move assignment calls `Empty()` before taking ownership of the source array’s storage.
- `Updated:` Initializer-list and copy constructors clean up allocated storage and constructed elements if construction fails.
- `Updated:` `DestroyElements()` destroys active elements in reverse order.

### Element Access
- `Added:` `T& Front() noexcept` returns a reference to the first element. Requires a nonempty array.
- `Fixed:` Both `At()` overloads reject `Index >= Size_`.
- `Updated:` `Num()` now returns `uint32`, replacing the previous implementation’s `uint8` return type.
  
### Capacity and Resizing
- `Updated:` `Reserve()` delegates storage allocation to `Reallocate()`.
- `Updated:` Removed `noexcept` from `Reserve()` so allocation and construction exceptions can propagate.
- `Fixed:` `Resize()` constructs new elements when growing and destroys removed elements when shrinking.
- `Added:` `Resize()` destroys newly constructed elements and restores the previous size if construction fails. Expanded capacity may be retained.
- `Updated:` `Resize()` explicitly updates `Size_` during destruction and rollback.
- `Added:` Capacity-limit checks in `Grow()`, `Emplace()`, and `Reallocate()` throw `std::length_error` for unsupported capacity requests.
- `Updated:` `Reallocate()` uses `std::move_if_noexcept` when transferring existing elements and cleans up partially constructed replacement storage on failure.

### Element Insertion and Removal
- `Updated:` Both `Add()` overloads delegate element construction to `Emplace()`.
- `Updated:` During growth, `Emplace()` constructs the appended element before relocating existing elements, allowing insertion arguments to reference existing array elements.
- `Fixed:` `Pop()` destroys the last element when removing it.
- `Updated:` `PopBack()` returns the removed element by value instead of by reference.
- `Added:` `PopBack()` throws `std::out_of_range` when the array is empty.
  
### Iterators
- `Updated:` `End()` and `CEnd()` return Data_ directly when the array is empty, avoiding arithmetic on a null pointer.