using UnityEngine;

public class MakeStone : MonoBehaviour
{
    public GameObject stone;
    public GameObject apple; // 🆕 사과 프리팹 연결용

    void Start()
    {
        InvokeRepeating("makeObject", 1.0f, 1.0f); // 1초마다 생성 시도
    }

    void makeObject()
    {
        int num = Random.Range(0, 3); // 0, 1, 2 중 랜덤
        if (num == 0) // 생성 조건: 1/3 확률
        {
            Vector3 pos = transform.position;
            pos.y = Random.Range(-2.0f, 2.5f); // y 위치 랜덤화

            int itemType = Random.Range(0, 2); // 0 또는 1
            if (itemType == 0 && stone != null)
            {
                Instantiate(stone, pos, transform.rotation);
            }
            else if (itemType == 1 && apple != null)
            {
                Instantiate(apple, pos, transform.rotation);
            }
        }
    }
}