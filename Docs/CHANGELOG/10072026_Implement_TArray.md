# CHANGELOG October - 7, 2026

# **JsonValue.h/cpp**
- `Updated:` Added int at index 2 for JsonVariant
- `Added:` Member method for JsonVariant<int> AsInt() const

# **TArray.h/inl**

### **Added:** Added full TArray to replace std::vector<T>

### **Members**
- `T* Data_` 
- `Size_T Capacity_` 
- `Size_T Size_`

### **Constructors:**
- `TArray<T>::TArray()`
- `TArray<T>::TArray(std::initializer_list<T> list)`
- `TArray<T>::TArray(const TArray& other)`
- `TArray<T>::TArray(TArray&& other) noexcept`

### **Destructor**
- `TArray<T>::~TArray()` 

### **Operators**
- `TArray<T>& TArray<T>::operator=(const TArray &other)`
- `TArray<T>& TArray<T>::operator=(TArray&& other) noexcept`
- `T& TArray<T>::operator[](Size_T index) noexcept`
- `const T& TArray<T>::operator[](Size_T index) const noexcept`


### **Methods**

#### **Element Access**

- `T& TArray<T>::At(Size_T index)`
	- Returns a reference to the element at the specified index. Throws `std::out_of_range` if the index exceeds `Size_`

- `const T& TArray<T>::At(Size_T index) const`
	- const overload for `At(Size_T index)`

- `T& TArray<T>::Back() noexcept`
	- Returns a reference to the very last element currently stored in the array

- `T* TArray<T>::GetData() noexcept`
	- Returns a direct raw pointer to the underlying data buffer

- `const T* TArray<T>::GetData() const noexcept`
	- const overload for `GetData()`

#### **Capacity and State Tracking**

- `Size_T TArray<T>::Num() const noexcept`
	- Returns the current number of active elements in the `TArray`
- `Size_T TArray<T>::Size() const noexcept`
	- Returns the current number of active elements in the `TArray`

- `Size_T TArray<T>::Max() const noexcept`
	- Returns the maximum number of elements `(Capacity_)` the array can hold before needing to reallocate

- `bool TArray<T>::IsEmpty() const noexcept`
	Returns true if the array contains zero elements

#### **Modifying and Structuring Storage**

- `void TArray<T>::Swap(TArray &other) noexcept`
	- Swaps the internal pointers, size, and capacity metrics of two `TArray` instances in constant time

- `void TArray<T>::Reserve(const Size_T newCapacity) noexcept`
	- Allocates a new chunk of memory to match `newCapacity` and moves all existing elements from the old block to the new one
    - No action is taken if newCapacity is less than or equal to current capacity

- `void TArray<T>::Resize(const Size_T newSize)`
	- Adjusts the logical Size_ of the array to `newSize`
    - Automatically calls `Reserve` to expand the buffer if `newSize` exceeds `Capacity_`

- `void TArray<T>::Shrink()`
	- Shrinks `Capacity_` down to match the exact current `Size_`
    - Fully empties the buffer if `Size_` is `0`

#### **Element Insertion amd Removal**

- `T& TArray<T>::Emplace(ArgumentTypes&&... arguments)`
    -  Initializes a new element in place at the end of the array
    - Triggers `Grow()` if the buffer is full

- `void TArray<T>::Add(const T& element)`
	- Appends a copy of the given element to the end of the array
    - Triggers `Grow()` if the buffer is full

- `void TArray<T>::Add(T&& element)`
	- Appends a moved instance of the given element to the end of the array
    - Triggers `Grow()` if the buffer is full

- `void TArray<T>::Pop()`
	- Decrements the logical size by 1 to remove the last element, assuming the array is not empty

- `T& TArray<T>::PopBack()`
    - Decrements the logical size by 1 and returns a reference to the element that was just dropped out of scope

#### **Memory Cleanup**

- `void TArray<T>::Clear() noexcept`
	- Invokes `DestroyElements()` to call explicit destructors on all active objects
    - Resets `Size_` to 0 while keeping the allocated capacity buffer intact

- `void TArray<T>::Empty() noexcept`
	- Destroys all active elements, frees the underlying data buffer
    - Zeros out `Data_`, `Size_`, and `Capacity_`

#### Iterators

- `T* TArray<T>::Begin() noexcept`
	- Returns an iterator `(pointer)` pointing to the first element in the array

- `const T* TArray<T>::Begin() const noexcept`
	- const overload for `Begin()`
- `const T* TArray<T>::CBegin() const noexcept`
	- Explicitly returns a constant iterator pointing to the first element in the array

- `T* TArray<T>::End() noexcept`
	- Returns an iterator `(pointer)` pointing to the memory slot directly past the last active element

- `const T* TArray<T>::End() const noexcept`
	- const overload for `End()`

- `const T* TArray<T>::CEnd() const noexcept`
	- Explicitly returns a constant iterator pointing to the memory slot directly past the last active element

- `void TArray<T>::Grow()`
	- Calculates a new capacity boundary (either initializing it to 1 or doubling the current capacity) and passes that value into `Reallocate()`

- `void TArray<T>::Reallocate(Size_T newCapacity)`
	- Allocates a new heap buffer of size `newCapacity`, moves all valid data from the old buffer over, releases the old block, and updates internal array metrics

- `void TArray<T>::DestroyElements() noexcept`
	- Iterates from index 0 up to `Size_` - 1 to explicitly call the destructor on every initialized object in the array



