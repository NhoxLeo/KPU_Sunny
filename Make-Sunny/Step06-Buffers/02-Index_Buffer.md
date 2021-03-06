# IndexBuffer

인덱스 버퍼는 정점 버퍼와 관련이 있다. 정점 버퍼에서 말 했듯이 3차원 물체의 기본 구축 요소는 삼각형이다. 예를 들어, 팔각형 하나를 삼각형 목록을 이용해서(즉, 점점 세  개다 삼각형 하나) 구축하기 위해 다음과 같은 정점 배열들을 사용한다고 하자.

```cpp
Vertex octagon[24] =
{
  v0, v1, v2, // 삼각형 0
  v0, v2, v3, // 삼각형 1
  v0, v3, v4, // 삼각형 2
  v0, v4, v5, // 삼각형 3
  v0, v5, v6, // 삼각형 4
  v0, v6, v7, // 삼각형 5
  v0, v7, v8, // 삼각형 6
  v0, v8, v1  // 삼각형 7
};
```

위처럼 하나의 3차원 물체를 형성하는 삼각형들은 다수의 정점들은 공유한다. 정점 두 개가 중복되는 것은 그리 큰 문제가 아니지만, 위 팔각형의 경우에는 문제가 심각하다. 팔각형은 모든 삼가형이 중심 점점 v0을 공유하며, 인접한 두 삼각형이 각각 팔각형 가장자리의 한 정점을 공유한다. 일반적으로 모형이 세밀하고 복잡할수록 중복되는 정점들도 많아진다.

정점들이 중복이 바람직하지 않은 이유는 크게 두 가지다.
- 메모리 요구량이 증가한다. (같은 정점 자료를 여러 번 저장하므로)
- 그래픽 하드웨어 처리량이 증가한다. (같은 정점 자료를 여러 번 처리하므로)

따라서 삼각형 목록에서 중복 점정들을 제거하는 방법으로 index를 사용 한다. 고유한 정점들로 정점 목록을 만들어 두고, 어떤 정점들을 어떤 순서로 사용해서 삼각형을 형성하는지 그 정점들의 색인들을 적절히 나열함으로써 지정하면 된다.

```cpp
Vertex v[9] = {v0, v1, v2, v3, v4, v5, v6, v7, v8};

unsigned int indexList[24] =
{
  0, 1, 2, // 삼각형 0
  0, 2, 3, // 삼각형 1
  0, 3, 4, // 삼각형 2
  0, 4, 5, // 삼각형 3
  0, 5, 6, // 삼각형 4
  0, 6, 7, // 삼각형 5
  0, 7, 8, // 삼각형 6
  0, 8, 0  // 삼각형 7
};
```

---

인덱스 역시 GPU가 접근할 수 있는 특별한 자원에 넣어야 한다. 그 자원이 바로 인덱스 버퍼다. 인덱스 버퍼의 생성은 정점 버퍼의 생성과 거의 비슷하다.

---

Sunny Engine은 인덱스 버퍼의 생성 과정을 추상화한 IndexBuffer 클래스가 존재합니다.

자세한 사용법은 cpp 파일에서 주석으로 대신합니다.

```cpp
class IndexBuffer
{
private:
	ID3D11Buffer* m_bufferHandle;

	unsigned int m_count;

private:
	IndexBuffer(unsigned int* data, unsigned int count);

	void Bind() const;

	inline unsigned int GetCount() const { return m_count; };
};
```

---

### Commit
[Step 06. Buffers](https://github.com/adunStudio/KPU_Sunny/commit/2848ec848c487c0be9d535ecc81add5c8f61f9ba)
