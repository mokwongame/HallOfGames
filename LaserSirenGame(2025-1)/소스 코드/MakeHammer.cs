using UnityEngine;

public class MakeHammer : MonoBehaviour
{
    private GameObject axisX;
    private GameObject axisY;

    public GameObject hammer;


    private void Start()
    {
        axisX = GameObject.Find("AxisX");
        axisY = GameObject.Find("AxisY");
    }

    void Update()
    {
        if (Input.GetKeyDown(KeyCode.Space))
        {
            makeHammer();
        }
    }
    void makeHammer()
    {
        Vector3 pos = new Vector3(axisX.transform.position.x, hammer.transform.position.y, axisY.transform.position.z);
        GameObject temp = Instantiate(hammer, pos, hammer.transform.rotation);
    }
}
