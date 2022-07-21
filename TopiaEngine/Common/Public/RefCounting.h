#pragma once

#include <atomic>
#include <cstddef>
#include <cassert>
#include <type_traits>

namespace topia
{
	template <typename T>
	class TRefCountPtr
	{
	public:
		typedef T InterfaceType;

		template <bool b, typename U = void>
		struct EnableIf
		{
		};

		template <typename U>
		struct EnableIf<true, U>
		{
			typedef U type;
		};

	protected:
		InterfaceType* ptr_;
		template <class U>
		friend class TRefCountPtr;

		void InternalAddRef() const noexcept
		{
			if (ptr_ != nullptr)
			{
				ptr_->AddRef();
			}
		}

		unsigned long InternalRelease() noexcept
		{
			unsigned long ref = 0;
			T* temp = ptr_;

			if (temp != nullptr)
			{
				ptr_ = nullptr;
				ref = temp->Release();
			}

			return ref;
		}

	public:
		TRefCountPtr() noexcept : ptr_(nullptr) {}

		TRefCountPtr(std::nullptr_t) noexcept : ptr_(nullptr) {}

		template <class U>
		TRefCountPtr(U* other) noexcept : ptr_(other)
		{
			InternalAddRef();
		}

		TRefCountPtr(const TRefCountPtr& other) noexcept : ptr_(other.ptr_) { InternalAddRef(); }

		// copy ctor that allows to instanatiate class when U* is convertible to T*
		template <class U>
		TRefCountPtr(const TRefCountPtr<U>& other,
		             typename std::enable_if<std::is_convertible<U*, T*>::value, void*>::type* = nullptr) noexcept
		    : ptr_(other.ptr_)

		{
			InternalAddRef();
		}

		TRefCountPtr(TRefCountPtr&& other) noexcept : ptr_(nullptr)
		{
			if (this != reinterpret_cast<TRefCountPtr*>(&reinterpret_cast<unsigned char&>(other)))
			{
				Swap(other);
			}
		}

		// Move ctor that allows instantiation of a class when U* is convertible to T*
		template <class U>
		TRefCountPtr(TRefCountPtr<U>&& other,
		             typename std::enable_if<std::is_convertible<U*, T*>::value, void*>::type* = nullptr) noexcept
		    : ptr_(other.ptr_)
		{
			other.ptr_ = nullptr;
		}

		~TRefCountPtr() noexcept { InternalRelease(); }

		TRefCountPtr& operator=(std::nullptr_t) noexcept
		{
			InternalRelease();
			return *this;
		}

		TRefCountPtr& operator=(T* other) noexcept
		{
			if (ptr_ != other)
			{
				TRefCountPtr(other).Swap(*this);
			}
			return *this;
		}

		template <typename U>
		TRefCountPtr& operator=(U* other) noexcept
		{
			TRefCountPtr(other).Swap(*this);
			return *this;
		}

		TRefCountPtr& operator=(const TRefCountPtr& other) noexcept // NOLINT(bugprone-unhandled-self-assignment)
		{
			if (ptr_ != other.ptr_)
			{
				TRefCountPtr(other).Swap(*this);
			}
			return *this;
		}

		template <class U>
		TRefCountPtr& operator=(const TRefCountPtr<U>& other) noexcept
		{
			TRefCountPtr(other).Swap(*this);
			return *this;
		}

		TRefCountPtr& operator=(TRefCountPtr&& other) noexcept
		{
			TRefCountPtr(static_cast<TRefCountPtr&&>(other)).Swap(*this);
			return *this;
		}

		template <class U>
		TRefCountPtr& operator=(TRefCountPtr<U>&& other) noexcept
		{
			TRefCountPtr(static_cast<TRefCountPtr<U>&&>(other)).Swap(*this);
			return *this;
		}

		void Swap(TRefCountPtr&& r) noexcept
		{
			T* tmp = ptr_;
			ptr_ = r.ptr_;
			r.ptr_ = tmp;
		}

		void Swap(TRefCountPtr& r) noexcept
		{
			T* tmp = ptr_;
			ptr_ = r.ptr_;
			r.ptr_ = tmp;
		}

		[[nodiscard]] T* Get() const noexcept { return ptr_; }

		operator T*() const { return ptr_; }

		InterfaceType* operator->() const noexcept { return ptr_; }

		T** operator&() // NOLINT(google-runtime-operator)
		{
			return &ptr_;
		}

		[[nodiscard]] T* const* GetAddressOf() const noexcept { return &ptr_; }

		[[nodiscard]] T** GetAddressOf() noexcept { return &ptr_; }

		[[nodiscard]] T** ReleaseAndGetAddressOf() noexcept
		{
			InternalRelease();
			return &ptr_;
		}

		T* Detach() noexcept
		{
			T* ptr = ptr_;
			ptr_ = nullptr;
			return ptr;
		}

		// Set the pointer while keeping the object's reference count unchanged
		void Attach(InterfaceType* other)
		{
			if (ptr_ != nullptr)
			{
				auto ref = ptr_->Release();
				(void)ref;

				// Attaching to the same object only works if duplicate references are being coalesced. Otherwise
				// re-attaching will cause the pointer to be released and may cause a crash on a subsequent dereference.
				assert(ref != 0 || ptr_ != other);
			}

			ptr_ = other;
		}

		// Create a wrapper around a raw object while keeping the object's reference count unchanged
		static TRefCountPtr<T> Create(T* other)
		{
			TRefCountPtr<T> Ptr;
			Ptr.Attach(other);
			return Ptr;
		}

		unsigned long Reset() { return InternalRelease(); }
	}; // TRefCountPtr

	template <typename T>
	class RefCounter : public T
	{
	private:
		std::atomic<unsigned long> ref_count_ = 1;

	public:
		virtual unsigned long AddRef() override { return ++ref_count_; }

		virtual unsigned long Release() override
		{
			unsigned long result = --ref_count_;
			if (result == 0)
			{
				delete this;
			}

			return result;
		}
	};
} // namespace topia
