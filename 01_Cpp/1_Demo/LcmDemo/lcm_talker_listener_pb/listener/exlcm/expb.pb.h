// Generated by the protocol buffer compiler.  DO NOT EDIT!
// source: expb.proto

#ifndef GOOGLE_PROTOBUF_INCLUDED_expb_2eproto
#define GOOGLE_PROTOBUF_INCLUDED_expb_2eproto

#include <limits>
#include <string>

#include <google/protobuf/port_def.inc>
#if PROTOBUF_VERSION < 3012000
#error This file was generated by a newer version of protoc which is
#error incompatible with your Protocol Buffer headers. Please update
#error your headers.
#endif
#if 3012002 < PROTOBUF_MIN_PROTOC_VERSION
#error This file was generated by an older version of protoc which is
#error incompatible with your Protocol Buffer headers. Please
#error regenerate this file with a newer version of protoc.
#endif

#include <google/protobuf/port_undef.inc>
#include <google/protobuf/io/coded_stream.h>
#include <google/protobuf/arena.h>
#include <google/protobuf/arenastring.h>
#include <google/protobuf/generated_message_table_driven.h>
#include <google/protobuf/generated_message_util.h>
#include <google/protobuf/inlined_string_field.h>
#include <google/protobuf/metadata_lite.h>
#include <google/protobuf/generated_message_reflection.h>
#include <google/protobuf/message.h>
#include <google/protobuf/repeated_field.h>  // IWYU pragma: export
#include <google/protobuf/extension_set.h>  // IWYU pragma: export
#include <google/protobuf/unknown_field_set.h>
// @@protoc_insertion_point(includes)
#include <google/protobuf/port_def.inc>
#define PROTOBUF_INTERNAL_EXPORT_expb_2eproto
PROTOBUF_NAMESPACE_OPEN
namespace internal {
class AnyMetadata;
}  // namespace internal
PROTOBUF_NAMESPACE_CLOSE

// Internal implementation detail -- do not use these members.
struct TableStruct_expb_2eproto {
  static const ::PROTOBUF_NAMESPACE_ID::internal::ParseTableField entries[]
    PROTOBUF_SECTION_VARIABLE(protodesc_cold);
  static const ::PROTOBUF_NAMESPACE_ID::internal::AuxillaryParseTableField aux[]
    PROTOBUF_SECTION_VARIABLE(protodesc_cold);
  static const ::PROTOBUF_NAMESPACE_ID::internal::ParseTable schema[1]
    PROTOBUF_SECTION_VARIABLE(protodesc_cold);
  static const ::PROTOBUF_NAMESPACE_ID::internal::FieldMetadata field_metadata[];
  static const ::PROTOBUF_NAMESPACE_ID::internal::SerializationTable serialization_table[];
  static const ::PROTOBUF_NAMESPACE_ID::uint32 offsets[];
};
extern const ::PROTOBUF_NAMESPACE_ID::internal::DescriptorTable descriptor_table_expb_2eproto;
namespace example {
class pos;
class posDefaultTypeInternal;
extern posDefaultTypeInternal _pos_default_instance_;
}  // namespace example
PROTOBUF_NAMESPACE_OPEN
template<> ::example::pos* Arena::CreateMaybeMessage<::example::pos>(Arena*);
PROTOBUF_NAMESPACE_CLOSE
namespace example {

// ===================================================================

class pos PROTOBUF_FINAL :
    public ::PROTOBUF_NAMESPACE_ID::Message /* @@protoc_insertion_point(class_definition:example.pos) */ {
 public:
  inline pos() : pos(nullptr) {};
  virtual ~pos();

  pos(const pos& from);
  pos(pos&& from) noexcept
    : pos() {
    *this = ::std::move(from);
  }

  inline pos& operator=(const pos& from) {
    CopyFrom(from);
    return *this;
  }
  inline pos& operator=(pos&& from) noexcept {
    if (GetArena() == from.GetArena()) {
      if (this != &from) InternalSwap(&from);
    } else {
      CopyFrom(from);
    }
    return *this;
  }

  inline const ::PROTOBUF_NAMESPACE_ID::UnknownFieldSet& unknown_fields() const {
    return _internal_metadata_.unknown_fields<::PROTOBUF_NAMESPACE_ID::UnknownFieldSet>(::PROTOBUF_NAMESPACE_ID::UnknownFieldSet::default_instance);
  }
  inline ::PROTOBUF_NAMESPACE_ID::UnknownFieldSet* mutable_unknown_fields() {
    return _internal_metadata_.mutable_unknown_fields<::PROTOBUF_NAMESPACE_ID::UnknownFieldSet>();
  }

  static const ::PROTOBUF_NAMESPACE_ID::Descriptor* descriptor() {
    return GetDescriptor();
  }
  static const ::PROTOBUF_NAMESPACE_ID::Descriptor* GetDescriptor() {
    return GetMetadataStatic().descriptor;
  }
  static const ::PROTOBUF_NAMESPACE_ID::Reflection* GetReflection() {
    return GetMetadataStatic().reflection;
  }
  static const pos& default_instance();

  static void InitAsDefaultInstance();  // FOR INTERNAL USE ONLY
  static inline const pos* internal_default_instance() {
    return reinterpret_cast<const pos*>(
               &_pos_default_instance_);
  }
  static constexpr int kIndexInFileMessages =
    0;

  friend void swap(pos& a, pos& b) {
    a.Swap(&b);
  }
  inline void Swap(pos* other) {
    if (other == this) return;
    if (GetArena() == other->GetArena()) {
      InternalSwap(other);
    } else {
      ::PROTOBUF_NAMESPACE_ID::internal::GenericSwap(this, other);
    }
  }
  void UnsafeArenaSwap(pos* other) {
    if (other == this) return;
    GOOGLE_DCHECK(GetArena() == other->GetArena());
    InternalSwap(other);
  }

  // implements Message ----------------------------------------------

  inline pos* New() const final {
    return CreateMaybeMessage<pos>(nullptr);
  }

  pos* New(::PROTOBUF_NAMESPACE_ID::Arena* arena) const final {
    return CreateMaybeMessage<pos>(arena);
  }
  void CopyFrom(const ::PROTOBUF_NAMESPACE_ID::Message& from) final;
  void MergeFrom(const ::PROTOBUF_NAMESPACE_ID::Message& from) final;
  void CopyFrom(const pos& from);
  void MergeFrom(const pos& from);
  PROTOBUF_ATTRIBUTE_REINITIALIZES void Clear() final;
  bool IsInitialized() const final;

  size_t ByteSizeLong() const final;
  const char* _InternalParse(const char* ptr, ::PROTOBUF_NAMESPACE_ID::internal::ParseContext* ctx) final;
  ::PROTOBUF_NAMESPACE_ID::uint8* _InternalSerialize(
      ::PROTOBUF_NAMESPACE_ID::uint8* target, ::PROTOBUF_NAMESPACE_ID::io::EpsCopyOutputStream* stream) const final;
  int GetCachedSize() const final { return _cached_size_.Get(); }

  private:
  inline void SharedCtor();
  inline void SharedDtor();
  void SetCachedSize(int size) const final;
  void InternalSwap(pos* other);
  friend class ::PROTOBUF_NAMESPACE_ID::internal::AnyMetadata;
  static ::PROTOBUF_NAMESPACE_ID::StringPiece FullMessageName() {
    return "example.pos";
  }
  protected:
  explicit pos(::PROTOBUF_NAMESPACE_ID::Arena* arena);
  private:
  static void ArenaDtor(void* object);
  inline void RegisterArenaDtor(::PROTOBUF_NAMESPACE_ID::Arena* arena);
  public:

  ::PROTOBUF_NAMESPACE_ID::Metadata GetMetadata() const final;
  private:
  static ::PROTOBUF_NAMESPACE_ID::Metadata GetMetadataStatic() {
    ::PROTOBUF_NAMESPACE_ID::internal::AssignDescriptors(&::descriptor_table_expb_2eproto);
    return ::descriptor_table_expb_2eproto.file_level_metadata[kIndexInFileMessages];
  }

  public:

  // nested types ----------------------------------------------------

  // accessors -------------------------------------------------------

  enum : int {
    kIndexFieldNumber = 1,
    kXFieldNumber = 2,
    kYFieldNumber = 3,
  };
  // optional int64 index = 1;
  bool has_index() const;
  private:
  bool _internal_has_index() const;
  public:
  void clear_index();
  ::PROTOBUF_NAMESPACE_ID::int64 index() const;
  void set_index(::PROTOBUF_NAMESPACE_ID::int64 value);
  private:
  ::PROTOBUF_NAMESPACE_ID::int64 _internal_index() const;
  void _internal_set_index(::PROTOBUF_NAMESPACE_ID::int64 value);
  public:

  // optional double x = 2;
  bool has_x() const;
  private:
  bool _internal_has_x() const;
  public:
  void clear_x();
  double x() const;
  void set_x(double value);
  private:
  double _internal_x() const;
  void _internal_set_x(double value);
  public:

  // optional double y = 3;
  bool has_y() const;
  private:
  bool _internal_has_y() const;
  public:
  void clear_y();
  double y() const;
  void set_y(double value);
  private:
  double _internal_y() const;
  void _internal_set_y(double value);
  public:

  // @@protoc_insertion_point(class_scope:example.pos)
 private:
  class _Internal;

  template <typename T> friend class ::PROTOBUF_NAMESPACE_ID::Arena::InternalHelper;
  typedef void InternalArenaConstructable_;
  typedef void DestructorSkippable_;
  ::PROTOBUF_NAMESPACE_ID::internal::HasBits<1> _has_bits_;
  mutable ::PROTOBUF_NAMESPACE_ID::internal::CachedSize _cached_size_;
  ::PROTOBUF_NAMESPACE_ID::int64 index_;
  double x_;
  double y_;
  friend struct ::TableStruct_expb_2eproto;
};
// ===================================================================


// ===================================================================

#ifdef __GNUC__
  #pragma GCC diagnostic push
  #pragma GCC diagnostic ignored "-Wstrict-aliasing"
#endif  // __GNUC__
// pos

// optional int64 index = 1;
inline bool pos::_internal_has_index() const {
  bool value = (_has_bits_[0] & 0x00000001u) != 0;
  return value;
}
inline bool pos::has_index() const {
  return _internal_has_index();
}
inline void pos::clear_index() {
  index_ = PROTOBUF_LONGLONG(0);
  _has_bits_[0] &= ~0x00000001u;
}
inline ::PROTOBUF_NAMESPACE_ID::int64 pos::_internal_index() const {
  return index_;
}
inline ::PROTOBUF_NAMESPACE_ID::int64 pos::index() const {
  // @@protoc_insertion_point(field_get:example.pos.index)
  return _internal_index();
}
inline void pos::_internal_set_index(::PROTOBUF_NAMESPACE_ID::int64 value) {
  _has_bits_[0] |= 0x00000001u;
  index_ = value;
}
inline void pos::set_index(::PROTOBUF_NAMESPACE_ID::int64 value) {
  _internal_set_index(value);
  // @@protoc_insertion_point(field_set:example.pos.index)
}

// optional double x = 2;
inline bool pos::_internal_has_x() const {
  bool value = (_has_bits_[0] & 0x00000002u) != 0;
  return value;
}
inline bool pos::has_x() const {
  return _internal_has_x();
}
inline void pos::clear_x() {
  x_ = 0;
  _has_bits_[0] &= ~0x00000002u;
}
inline double pos::_internal_x() const {
  return x_;
}
inline double pos::x() const {
  // @@protoc_insertion_point(field_get:example.pos.x)
  return _internal_x();
}
inline void pos::_internal_set_x(double value) {
  _has_bits_[0] |= 0x00000002u;
  x_ = value;
}
inline void pos::set_x(double value) {
  _internal_set_x(value);
  // @@protoc_insertion_point(field_set:example.pos.x)
}

// optional double y = 3;
inline bool pos::_internal_has_y() const {
  bool value = (_has_bits_[0] & 0x00000004u) != 0;
  return value;
}
inline bool pos::has_y() const {
  return _internal_has_y();
}
inline void pos::clear_y() {
  y_ = 0;
  _has_bits_[0] &= ~0x00000004u;
}
inline double pos::_internal_y() const {
  return y_;
}
inline double pos::y() const {
  // @@protoc_insertion_point(field_get:example.pos.y)
  return _internal_y();
}
inline void pos::_internal_set_y(double value) {
  _has_bits_[0] |= 0x00000004u;
  y_ = value;
}
inline void pos::set_y(double value) {
  _internal_set_y(value);
  // @@protoc_insertion_point(field_set:example.pos.y)
}

#ifdef __GNUC__
  #pragma GCC diagnostic pop
#endif  // __GNUC__

// @@protoc_insertion_point(namespace_scope)

}  // namespace example

// @@protoc_insertion_point(global_scope)

#include <google/protobuf/port_undef.inc>
#endif  // GOOGLE_PROTOBUF_INCLUDED_GOOGLE_PROTOBUF_INCLUDED_expb_2eproto
