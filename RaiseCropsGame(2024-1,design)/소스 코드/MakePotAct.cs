using System.Collections;
using System.Collections.Generic;
using UnityEngine;

public class MakePotAct : MonoBehaviour
{
    public GameObject Pot;
    float xmin = 0.0f;
    float xmax = 9.0f;
    float ymin = -4.0f;
    float ymax = 4.0f;

    // Start is called before the first frame update
    void Start()
    {
        InvokeRepeating("MakePot", 1.0f, 3.0f);  // �޼��� �̸��� ��Ȯ�� ����
    }

    // Update is called once per frame
    void Update()
    {

    }

    void MakePot()  // �޼��� �̸��� ��ҹ��� �����Ͽ� ����
    {
        Vector3 pos = new Vector3();
        pos.x = Random.Range(xmin, xmax);
        pos.y = Random.Range(ymin, ymax);
        pos.z = 0.0f;
        Instantiate(Pot, pos, transform.rotation);
    }
}
