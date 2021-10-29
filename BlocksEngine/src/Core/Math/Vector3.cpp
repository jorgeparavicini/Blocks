#include "BlocksEngine/pch.h"
#include "BlocksEngine/Vector3.h"

#include <boost/functional/hash.hpp>

using namespace DirectX;

namespace BlocksEngine
{
    //------------------------------------------------------------------------------
    // Constructors
    //------------------------------------------------------------------------------

    Vector3::Vector3() noexcept
        : XMFLOAT3{0.0f, 0.0f, 0.0f}
    {
    }

    Vector3::Vector3(_In_reads_(3) const float* pArray) noexcept
        : XMFLOAT3{pArray}
    {
    }

    Vector3::Vector3(FXMVECTOR v) noexcept // NOLINT(cppcoreguidelines-pro-type-member-init)
    {
        XMStoreFloat3(this, v);
    }

    Vector3::Vector3(const XMFLOAT3& v) noexcept
        : XMFLOAT3{v.x, v.y, v.z}
    {
    }

    Vector3::Vector3(const XMVECTORF32& v) noexcept
        : XMFLOAT3{v.f[0], v.f[1], v.f[2]}
    {
    }

    //------------------------------------------------------------------------------
    // Converter operators
    //------------------------------------------------------------------------------

    Vector3::operator XMVECTOR() const noexcept
    {
        return XMLoadFloat3(this);
    }

    //------------------------------------------------------------------------------
    // Comparision operators
    //------------------------------------------------------------------------------

    bool Vector3::operator==(const Vector3& v) const noexcept
    {
        const XMVECTOR v1 = XMLoadFloat3(this);
        const XMVECTOR v2 = XMLoadFloat3(&v);
        return XMVector3Equal(v1, v2);
    }

    bool Vector3::operator!=(const Vector3& v) const noexcept
    {
        const XMVECTOR v1 = XMLoadFloat3(this);
        const XMVECTOR v2 = XMLoadFloat3(&v);
        return XMVector3NotEqual(v1, v2);
    }

    //------------------------------------------------------------------------------
    // Assignment operators
    //------------------------------------------------------------------------------

    Vector3& Vector3::operator=(const XMVECTORF32& v) noexcept
    {
        x = v.f[0];
        y = v.f[1];
        z = v.f[2];

        return *this;
    }

    Vector3& Vector3::operator+=(const Vector3& v) noexcept
    {
        const XMVECTOR v1 = XMLoadFloat3(this);
        const XMVECTOR v2 = XMLoadFloat3(&v);
        const XMVECTOR x = XMVectorAdd(v1, v2);
        XMStoreFloat3(this, x);

        return *this;
    }

    Vector3& Vector3::operator-=(const Vector3& v) noexcept
    {
        const XMVECTOR v1 = XMLoadFloat3(this);
        const XMVECTOR v2 = XMLoadFloat3(&v);
        const XMVECTOR x = XMVectorSubtract(v1, v2);
        XMStoreFloat3(this, x);

        return *this;
    }

    Vector3& Vector3::operator*=(const Vector3& v) noexcept
    {
        const XMVECTOR v1 = XMLoadFloat3(this);
        const XMVECTOR v2 = XMLoadFloat3(&v);
        const XMVECTOR x = XMVectorMultiply(v1, v2);
        XMStoreFloat3(this, x);

        return *this;
    }

    Vector3& Vector3::operator*=(const float s) noexcept
    {
        const XMVECTOR v1 = XMLoadFloat3(this);
        const XMVECTOR x = XMVectorScale(v1, s);
        XMStoreFloat3(this, x);

        return *this;
    }

    Vector3& Vector3::operator/=(const float s) noexcept
    {
        const XMVECTOR v1 = XMLoadFloat3(this);
        const XMVECTOR x = XMVectorScale(v1, 1.0f / s);
        XMStoreFloat3(this, x);

        return *this;
    }

    //------------------------------------------------------------------------------
    // Unary operators
    //------------------------------------------------------------------------------

    Vector3 Vector3::operator+() const noexcept
    {
        return *this;
    }

    Vector3 Vector3::operator-() const noexcept
    {
        const XMVECTOR v1 = XMLoadFloat3(this);
        const XMVECTOR x = XMVectorNegate(v1);
        Vector3 r;
        XMStoreFloat3(&r, x);
        return r;
    }

    //------------------------------------------------------------------------------
    // Hashing
    //------------------------------------------------------------------------------

    std::size_t hash_value(const Vector3& v)
    {
        std::size_t seed = 0;
        boost::hash_combine(seed, v.x);
        boost::hash_combine(seed, v.y);
        boost::hash_combine(seed, v.z);

        return seed;
    }

    //------------------------------------------------------------------------------
    // Vector operations
    //------------------------------------------------------------------------------

    bool Vector3::InBounds(const Vector3& bounds) const noexcept
    {
        const XMVECTOR v1 = XMLoadFloat3(this);
        const XMVECTOR v2 = XMLoadFloat3(&bounds);
        return XMVector3InBounds(v1, v2);
    }

    float Vector3::Length() const noexcept
    {
        const XMVECTOR v1 = XMLoadFloat3(this);
        const XMVECTOR x = XMVector3Length(v1);
        return XMVectorGetX(x);
    }

    float Vector3::LengthSquared() const noexcept
    {
        const XMVECTOR v1 = XMLoadFloat3(this);
        const XMVECTOR x = XMVector3LengthSq(v1);
        return XMVectorGetX(x);
    }

    float Vector3::Dot(const Vector3& v) const noexcept
    {
        const XMVECTOR v1 = XMLoadFloat3(this);
        const XMVECTOR v2 = XMLoadFloat3(&v);
        const XMVECTOR x = XMVector3Dot(v1, v2);
        return XMVectorGetX(x);
    }

    void Vector3::Cross(const Vector3& v, Vector3& result) const noexcept
    {
        const XMVECTOR v1 = XMLoadFloat3(this);
        const XMVECTOR v2 = XMLoadFloat3(&v);
        const XMVECTOR x = XMVector3Cross(v1, v2);
        XMStoreFloat3(&result, x);
    }

    Vector3 Vector3::Cross(const Vector3& v) const noexcept
    {
        const XMVECTOR v1 = XMLoadFloat3(this);
        const XMVECTOR v2 = XMLoadFloat3(&v);
        const XMVECTOR x = XMVector3Cross(v1, v2);

        Vector3 result;
        XMStoreFloat3(&result, x);
        return result;
    }

    void Vector3::Normalize() noexcept
    {
        const XMVECTOR v1 = XMLoadFloat3(this);
        const XMVECTOR x = XMVector3Normalize(v1);
        XMStoreFloat3(this, x);
    }

    void Vector3::Normalize(Vector3& result) const noexcept
    {
        const XMVECTOR v1 = XMLoadFloat3(this);
        const XMVECTOR x = XMVector3Normalize(v1);
        XMStoreFloat3(&result, x);
    }

    void Vector3::Clamp(const Vector3& vMin, const Vector3& vMax) noexcept
    {
        const XMVECTOR v1 = XMLoadFloat3(this);
        const XMVECTOR v2 = XMLoadFloat3(&vMin);
        const XMVECTOR v3 = XMLoadFloat3(&vMax);
        const XMVECTOR x = XMVectorClamp(v1, v2, v3);
        XMStoreFloat3(this, x);
    }

    void Vector3::Clamp(const Vector3& vMin, const Vector3& vMax, Vector3& result) const noexcept
    {
        const XMVECTOR v1 = XMLoadFloat3(this);
        const XMVECTOR v2 = XMLoadFloat3(&vMin);
        const XMVECTOR v3 = XMLoadFloat3(&vMax);
        const XMVECTOR x = XMVectorClamp(v1, v2, v3);
        XMStoreFloat3(&result, x);
    }

    //------------------------------------------------------------------------------
    // Static functions
    //------------------------------------------------------------------------------

    float Vector3::Distance(const Vector3& v1, const Vector3& v2) noexcept
    {
        const XMVECTOR x1 = XMLoadFloat3(&v1);
        const XMVECTOR x2 = XMLoadFloat3(&v2);
        const XMVECTOR v = XMVectorSubtract(x2, x1);
        const XMVECTOR x = XMVector3Length(v);
        return XMVectorGetX(x);
    }

    float Vector3::DistanceSquared(const Vector3& v1, const Vector3& v2) noexcept
    {
        const XMVECTOR x1 = XMLoadFloat3(&v1);
        const XMVECTOR x2 = XMLoadFloat3(&v2);
        const XMVECTOR v = XMVectorSubtract(x2, x1);
        const XMVECTOR x = XMVector3LengthSq(v);
        return XMVectorGetX(x);
    }

    void Vector3::Min(const Vector3& v1, const Vector3& v2, Vector3& result) noexcept
    {
        const XMVECTOR x1 = XMLoadFloat3(&v1);
        const XMVECTOR x2 = XMLoadFloat3(&v2);
        const XMVECTOR x = XMVectorMin(x1, x2);
        XMStoreFloat3(&result, x);
    }

    Vector3 Vector3::Min(const Vector3& v1, const Vector3& v2) noexcept
    {
        const XMVECTOR x1 = XMLoadFloat3(&v1);
        const XMVECTOR x2 = XMLoadFloat3(&v2);
        const XMVECTOR x = XMVectorMin(x2, x1);

        Vector3 result;
        XMStoreFloat3(&result, x);
        return result;
    }

    void Vector3::Max(const Vector3& v1, const Vector3& v2, Vector3& result) noexcept
    {
        const XMVECTOR x1 = XMLoadFloat3(&v1);
        const XMVECTOR x2 = XMLoadFloat3(&v2);
        const XMVECTOR x = XMVectorMax(x2, x1);
        XMStoreFloat3(&result, x);
    }

    Vector3 Vector3::Max(const Vector3& v1, const Vector3& v2) noexcept
    {
        const XMVECTOR x1 = XMLoadFloat3(&v1);
        const XMVECTOR x2 = XMLoadFloat3(&v2);
        const XMVECTOR x = XMVectorMax(x2, x1);

        Vector3 result;
        XMStoreFloat3(&result, x);
        return result;
    }

    void Vector3::Lerp(const Vector3& v1, const Vector3& v2, const float t, Vector3& result) noexcept
    {
        const XMVECTOR x1 = XMLoadFloat3(&v1);
        const XMVECTOR x2 = XMLoadFloat3(&v2);
        const XMVECTOR x = XMVectorLerp(x2, x1, t);
        XMStoreFloat3(&result, x);
    }

    Vector3 Vector3::Lerp(const Vector3& v1, const Vector3& v2, float t) noexcept
    {
        const XMVECTOR x1 = XMLoadFloat3(&v1);
        const XMVECTOR x2 = XMLoadFloat3(&v2);
        const XMVECTOR x = XMVectorLerp(x2, x1, t);

        Vector3 result;
        XMStoreFloat3(&result, x);
        return result;
    }

    void Vector3::SmoothStep(const Vector3& v1, const Vector3& v2, float t, Vector3& result) noexcept
    {
        t = t > 1.0f ? 1.0f : t < 0.0f ? 0.0f : t; // Clamp value between 0 and 1
        t = t * t * (3.0f - 2.0f * t);
        const XMVECTOR x1 = XMLoadFloat3(&v1);
        const XMVECTOR x2 = XMLoadFloat3(&v2);
        const XMVECTOR x = XMVectorLerp(x1, x2, t);
        XMStoreFloat3(&result, x);
    }

    Vector3 Vector3::SmoothStep(const Vector3& v1, const Vector3& v2, float t) noexcept
    {
        t = t > 1.0f ? 1.0f : t < 0.0f ? 0.0f : t; // Clamp value between 0 and 1
        t = t * t * (3.0f - 2.0f * t);
        const XMVECTOR x1 = XMLoadFloat3(&v1);
        const XMVECTOR x2 = XMLoadFloat3(&v2);
        const XMVECTOR x = XMVectorLerp(x1, x2, t);

        Vector3 result;
        XMStoreFloat3(&result, x);
        return result;
    }

    void Vector3::Barycentric(const Vector3& v1, const Vector3& v2, const Vector3& v3, const float f,
                              const float g, Vector3& result) noexcept
    {
        const XMVECTOR x1 = XMLoadFloat3(&v1);
        const XMVECTOR x2 = XMLoadFloat3(&v2);
        const XMVECTOR x3 = XMLoadFloat3(&v3);
        const XMVECTOR x = XMVectorBaryCentric(x1, x2, x3, f, g);
        XMStoreFloat3(&result, x);
    }

    Vector3 Vector3::Barycentric(const Vector3& v1, const Vector3& v2, const Vector3& v3,
                                 const float f, const float g) noexcept
    {
        const XMVECTOR x1 = XMLoadFloat3(&v1);
        const XMVECTOR x2 = XMLoadFloat3(&v2);
        const XMVECTOR x3 = XMLoadFloat3(&v3);
        const XMVECTOR x = XMVectorBaryCentric(x1, x2, x3, f, g);

        Vector3 result;
        XMStoreFloat3(&result, x);
        return result;
    }

    void Vector3::CatmullRom(const Vector3& v1, const Vector3& v2, const Vector3& v3, const Vector3& v4,
                             const float t, Vector3& result) noexcept
    {
        const XMVECTOR x1 = XMLoadFloat3(&v1);
        const XMVECTOR x2 = XMLoadFloat3(&v2);
        const XMVECTOR x3 = XMLoadFloat3(&v3);
        const XMVECTOR x4 = XMLoadFloat3(&v4);
        const XMVECTOR x = XMVectorCatmullRom(x1, x2, x3, x4, t);
        XMStoreFloat3(&result, x);
    }

    Vector3 Vector3::CatmullRom(const Vector3& v1, const Vector3& v2, const Vector3& v3,
                                const Vector3& v4, const float t) noexcept
    {
        const XMVECTOR x1 = XMLoadFloat3(&v1);
        const XMVECTOR x2 = XMLoadFloat3(&v2);
        const XMVECTOR x3 = XMLoadFloat3(&v3);
        const XMVECTOR x4 = XMLoadFloat3(&v4);
        const XMVECTOR x = XMVectorCatmullRom(x1, x2, x3, x4, t);

        Vector3 result;
        XMStoreFloat3(&result, x);
        return result;
    }

    void Vector3::Hermite(const Vector3& v1, const Vector3& t1, const Vector3& v2, const Vector3& t2, float t,
                          Vector3& result) noexcept
    {
        const XMVECTOR x1 = XMLoadFloat3(&v1);
        const XMVECTOR x2 = XMLoadFloat3(&t1);
        const XMVECTOR x3 = XMLoadFloat3(&v2);
        const XMVECTOR x4 = XMLoadFloat3(&t2);
        const XMVECTOR x = XMVectorHermite(x1, x2, x3, x4, t);

        XMStoreFloat3(&result, x);
    }

    Vector3 Vector3::Hermite(const Vector3& v1, const Vector3& t1, const Vector3& v2,
                             const Vector3& t2, float t) noexcept
    {
        const XMVECTOR x1 = XMLoadFloat3(&v1);
        const XMVECTOR x2 = XMLoadFloat3(&t1);
        const XMVECTOR x3 = XMLoadFloat3(&v2);
        const XMVECTOR x4 = XMLoadFloat3(&t2);
        const XMVECTOR x = XMVectorHermite(x1, x2, x3, x4, t);

        Vector3 result;
        XMStoreFloat3(&result, x);
        return result;
    }

    void Vector3::Reflect(const Vector3& iVec, const Vector3& nVec, Vector3& result) noexcept
    {
        const XMVECTOR x1 = XMLoadFloat3(&iVec);
        const XMVECTOR x2 = XMLoadFloat3(&nVec);
        const XMVECTOR x = XMVector3Reflect(x1, x2);

        XMStoreFloat3(&result, x);
    }

    Vector3 Vector3::Reflect(const Vector3& iVec, const Vector3& nVec) noexcept
    {
        const XMVECTOR x1 = XMLoadFloat3(&iVec);
        const XMVECTOR x2 = XMLoadFloat3(&nVec);
        const XMVECTOR x = XMVector3Reflect(x1, x2);

        Vector3 result;
        XMStoreFloat3(&result, x);
        return result;
    }

    void Vector3::Refract(const Vector3& iVec, const Vector3& nVec, const float refractionIndex,
                          Vector3& result) noexcept
    {
        const XMVECTOR x1 = XMLoadFloat3(&iVec);
        const XMVECTOR x2 = XMLoadFloat3(&nVec);
        const XMVECTOR x = XMVector3Refract(x1, x2, refractionIndex);

        XMStoreFloat3(&result, x);
    }

    Vector3 Vector3::Refract(const Vector3& iVec, const Vector3& nVec,
                             const float refractionIndex) noexcept
    {
        const XMVECTOR x1 = XMLoadFloat3(&iVec);
        const XMVECTOR x2 = XMLoadFloat3(&nVec);
        const XMVECTOR x = XMVector3Refract(x1, x2, refractionIndex);

        Vector3 result;
        XMStoreFloat3(&result, x);
        return result;
    }

    void Vector3::Transform(const Vector3& v, const Quaternion& quat, Vector3& result) noexcept
    {
        const XMVECTOR x1 = XMLoadFloat3(&v);
        const XMVECTOR x2 = XMLoadFloat4(&quat);
        const XMVECTOR x = XMVector3Rotate(x1, x2);
        XMStoreFloat3(&result, x);
    }

    Vector3 Vector3::Transform(const Vector3& v, const Quaternion& quat) noexcept
    {
        const XMVECTOR x1 = XMLoadFloat3(&v);
        const XMVECTOR x2 = XMLoadFloat4(&quat);
        const XMVECTOR x = XMVector3Rotate(x1, x2);

        Vector3 result;
        XMStoreFloat3(&result, x);
        return result;
    }

    void Vector3::Transform(const Vector3& v, const Matrix& m, Vector3& result) noexcept
    {
        const XMVECTOR v1 = XMLoadFloat3(&v);
        const XMMATRIX m1 = XMLoadFloat4x4(&m);
        const XMVECTOR x = XMVector3TransformCoord(v1, m1);
        XMStoreFloat3(&result, x);
    }

    Vector3 Vector3::Transform(const Vector3& v, const Matrix& m) noexcept
    {
        const XMVECTOR v1 = XMLoadFloat3(&v);
        const XMMATRIX m1 = XMLoadFloat4x4(&m);
        const XMVECTOR x = XMVector3TransformCoord(v1, m1);

        Vector3 result;
        XMStoreFloat3(&result, x);
        return result;
    }

    _Use_decl_annotations_

    void Vector3::Transform(const Vector3* varray, const size_t count, const Matrix& m,
                            Vector3* resultArray) noexcept
    {
        const XMMATRIX m1 = XMLoadFloat4x4(&m);
        XMVector3TransformCoordStream(resultArray, sizeof XMFLOAT3, varray, sizeof XMFLOAT3, count, m1);
    }

    void Vector3::Transform(const Vector3& v, const Matrix& m, Vector4& result) noexcept
    {
        const XMVECTOR v1 = XMLoadFloat3(&v);
        const XMMATRIX m1 = XMLoadFloat4x4(&m);
        const XMVECTOR x = XMVector3Transform(v1, m1);
        XMStoreFloat4(&result, x);
    }

    _Use_decl_annotations_

    void Vector3::Transform(const Vector3* varray, const size_t count, const Matrix& m,
                            Vector4* resultArray) noexcept
    {
        const XMMATRIX m1 = XMLoadFloat4x4(&m);
        XMVector3TransformStream(resultArray, sizeof XMFLOAT4, varray, sizeof XMFLOAT3, count, m1);
    }

    void Vector3::TransformNormal(const Vector3& v, const Matrix& m, Vector3& result) noexcept
    {
        const XMVECTOR v1 = XMLoadFloat3(&v);
        const XMMATRIX m1 = XMLoadFloat4x4(&m);
        const XMVECTOR x = XMVector3TransformNormal(v1, m1);
        XMStoreFloat3(&result, x);
    }

    Vector3 Vector3::TransformNormal(const Vector3& v, const Matrix& m) noexcept
    {
        const XMVECTOR v1 = XMLoadFloat3(&v);
        const XMMATRIX m1 = XMLoadFloat4x4(&m);
        const XMVECTOR x = XMVector3TransformNormal(v1, m1);

        Vector3 result;
        XMStoreFloat3(&result, x);
        return result;
    }

    _Use_decl_annotations_

    void Vector3::TransformNormal(const Vector3* varray, const size_t count, const Matrix& m,
                                  Vector3* resultArray) noexcept
    {
        const XMMATRIX m1 = XMLoadFloat4x4(&m);
        XMVector3TransformNormalStream(resultArray, sizeof XMFLOAT3, varray, sizeof XMFLOAT3, count, m1);
    }

    //------------------------------------------------------------------------------
    // Constants
    //------------------------------------------------------------------------------

    const Vector3 Vector3::Zero = {0.f, 0.f, 0.f};
    const Vector3 Vector3::One = {1.f, 1.f, 1.f};
    const Vector3 Vector3::UnitX = {1.f, 0.f, 0.f};
    const Vector3 Vector3::UnitY = {0.f, 1.f, 0.f};
    const Vector3 Vector3::UnitZ = {0.f, 0.f, 1.f};
    const Vector3 Vector3::Up = {0.f, 1.f, 0.f};
    const Vector3 Vector3::Down = {0.f, -1.f, 0.f};
    const Vector3 Vector3::Right = {1.f, 0.f, 0.f};
    const Vector3 Vector3::Left = {-1.f, 0.f, 0.f};
    const Vector3 Vector3::Forward = {0.f, 0.f, 1.f};
    const Vector3 Vector3::Backward = {0.f, 0.f, -1.f};
}

//------------------------------------------------------------------------------
// Binary operators
//------------------------------------------------------------------------------

BlocksEngine::Vector3 operator+(const BlocksEngine::Vector3& v1, const BlocksEngine::Vector3& v2) noexcept
{
    const XMVECTOR x1 = XMLoadFloat3(&v1);
    const XMVECTOR x2 = XMLoadFloat3(&v2);
    const XMVECTOR x = XMVectorAdd(x1, x2);

    BlocksEngine::Vector3 result;
    XMStoreFloat3(&result, x);
    return result;
}

BlocksEngine::Vector3 operator-(const BlocksEngine::Vector3& v1, const BlocksEngine::Vector3& v2) noexcept
{
    const XMVECTOR x1 = XMLoadFloat3(&v1);
    const XMVECTOR x2 = XMLoadFloat3(&v2);
    const XMVECTOR x = XMVectorSubtract(x1, x2);

    BlocksEngine::Vector3 result;
    XMStoreFloat3(&result, x);
    return result;
}

BlocksEngine::Vector3 operator*(const BlocksEngine::Vector3& v1, const BlocksEngine::Vector3& v2) noexcept
{
    const XMVECTOR x1 = XMLoadFloat3(&v1);
    const XMVECTOR x2 = XMLoadFloat3(&v2);
    const XMVECTOR x = XMVectorMultiply(x1, x2);

    BlocksEngine::Vector3 result;
    XMStoreFloat3(&result, x);
    return result;
}

BlocksEngine::Vector3 operator*(const BlocksEngine::Vector3& v, const float s) noexcept
{
    const XMVECTOR x1 = XMLoadFloat3(&v);
    const XMVECTOR x = XMVectorScale(x1, s);

    BlocksEngine::Vector3 result;
    XMStoreFloat3(&result, x);
    return result;
}

BlocksEngine::Vector3 operator*(const BlocksEngine::Quaternion& q, const BlocksEngine::Vector3& v) noexcept
{
    const float x = q.x * 2.0f;
    const float y = q.y * 2.0f;
    const float z = q.z * 2.0f;
    const float xx = q.x * x;
    const float yy = q.y * y;
    const float zz = q.z * z;
    const float xy = q.x * y;
    const float xz = q.x * z;
    const float yz = q.y * z;
    const float wx = q.w * x;
    const float wy = q.w * y;
    const float wz = q.w * z;

    const float resX = (1.0f - (yy + zz)) * v.x + (xy - wz) * v.y + (xz + wy) * v.z;
    const float resY = (xy + wz) * v.x + (1.0f - (xx + zz)) * v.y + (yz - wx) * v.z;
    const float resZ = (xz - wy) * v.x + (yz + wx) * v.y + (1.0f - (xx + yy)) * v.z;

    BlocksEngine::Vector3 result;
    XMStoreFloat3(&result, XMVectorSet(resX, resY, resZ, 0));
    return result;
}

BlocksEngine::Vector3 operator/(const BlocksEngine::Vector3& v1, const BlocksEngine::Vector3& v2) noexcept
{
    const XMVECTOR x1 = XMLoadFloat3(&v1);
    const XMVECTOR x2 = XMLoadFloat3(&v2);
    const XMVECTOR x = XMVectorDivide(x1, x2);

    BlocksEngine::Vector3 result;
    XMStoreFloat3(&result, x);
    return result;
}

BlocksEngine::Vector3 operator/(const BlocksEngine::Vector3& v, const float s) noexcept
{
    const XMVECTOR x1 = XMLoadFloat3(&v);
    const XMVECTOR x = XMVectorScale(x1, 1.0f / s);

    BlocksEngine::Vector3 result;
    XMStoreFloat3(&result, x);
    return result;
}

BlocksEngine::Vector3 operator*(const float s, const BlocksEngine::Vector3& v) noexcept
{
    const XMVECTOR x1 = XMLoadFloat3(&v);
    const XMVECTOR x = XMVectorScale(x1, s);

    BlocksEngine::Vector3 result;
    XMStoreFloat3(&result, x);
    return result;
}
